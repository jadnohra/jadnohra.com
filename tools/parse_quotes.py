#!/usr/bin/env python3
"""
Parse quotes from 77-neurons-project-perelman HTML files
and categorize them by topic.
"""

import re
import json
from pathlib import Path
from html import unescape

# Jad's original tag system
JAD_TAGS = {
    "Justif": "Philosophical justifications",
    "Curious": "Curious and/or Prolix",
    "Origins": "Historical origins of concepts",
    "PhilTake": "Philosophical take on formal ideas",
    "AA": "The Amazing Ancients",
    "IKIT": "I Knew It!",
    "Sep": "Separate mathematician from work",
    "Clueless": "Brilliant but unaware of importance",
    "Humor": "Humor",
}

# Topic keywords for auto-categorization
TOPIC_KEYWORDS = {
    "Foundations": {
        "Logic": ["logic", "logical", "proposition", "predicate", "truth", "false", "excluded middle", "contradiction"],
        "Set Theory": ["set", "sets", "cardinality", "infinite", "countable", "uncountable"],
        "Proof": ["proof", "prove", "theorem", "lemma", "axiom", "deduce", "induction"],
    },
    "Analysis": {
        "Calculus": ["calculus", "derivative", "integral", "differentiate", "limit", "infinitesimal"],
        "Real Analysis": ["real analysis", "convergence", "sequence", "series", "continuous", "epsilon", "delta"],
        "Measure": ["measure", "lebesgue", "measurable"],
    },
    "Algebra": {
        "Linear Algebra": ["linear", "vector", "matrix", "eigenvalue", "dimension", "basis"],
        "Group Theory": ["group", "symmetry", "galois", "permutation", "homomorphism"],
        "Abstract Algebra": ["ring", "field", "algebra", "module"],
    },
    "Geometry": {
        "Differential Geometry": ["manifold", "curvature", "geodesic", "tensor", "riemannian"],
        "Topology": ["topology", "topological", "homeomorphism", "compact", "connected"],
        "Euclidean": ["euclidean", "triangle", "circle", "angle", "geometric"],
    },
    "Physics": {
        "Mechanics": ["mechanics", "force", "motion", "newton", "momentum", "energy", "lagrangian", "hamiltonian"],
        "Relativity": ["relativity", "einstein", "spacetime", "gravity", "gravitational"],
        "Quantum": ["quantum", "wave", "particle", "heisenberg", "schrodinger"],
    },
    "Philosophy": {
        "Nature of Math": ["philosophy", "mathematical", "abstract", "platonic", "formalism", "intuition"],
        "History": ["history", "ancient", "greek", "euler", "gauss", "kepler", "copernicus"],
        "Beauty": ["beauty", "elegant", "beautiful", "aesthetic", "simple", "profound"],
    },
}

def extract_quotes_from_html(html_content):
    """Extract quotes from HTML list items."""
    quotes = []

    # Find all <li> tags content
    li_pattern = re.compile(r'<li[^>]*>(.*?)</li>', re.DOTALL | re.IGNORECASE)

    for match in li_pattern.finditer(html_content):
        raw_text = match.group(1)

        # Skip navigation/tag definition items
        if 'sites-navigation' in raw_text or 'class="' in raw_text[:50]:
            continue
        if raw_text.startswith('<Tag') or raw_text.startswith('&lt;Tag'):
            continue

        # Clean HTML tags and entities
        text = re.sub(r'<[^>]+>', '', raw_text)
        text = unescape(text)
        text = re.sub(r'\s+', ' ', text).strip()

        # Skip short items or navigation
        if len(text) < 50 or text.startswith('Week ') or text.startswith('LinAlg'):
            continue

        # Try to extract source (usually in parentheses at the end)
        source_match = re.search(r'\(([^)]+)\)\s*$', text)
        source = source_match.group(1) if source_match else None

        # Extract any existing tags (from both text and raw HTML)
        tags = []
        for tag_match in re.findall(r'<(\w+)>', text):
            if tag_match in JAD_TAGS:
                tags.append(tag_match)
        for tag_match in re.findall(r'&lt;(\w+)&gt;', raw_text):
            if tag_match in JAD_TAGS and tag_match not in tags:
                tags.append(tag_match)

        quotes.append({
            'text': text,
            'source': source,
            'original_tags': tags,
            'topics': []
        })

    return quotes

def categorize_quote(quote):
    """Auto-categorize a quote based on keywords and original tags."""
    text_lower = quote['text'].lower()
    topics = []

    # Add Jad's System topics from original tags
    for tag in quote.get('original_tags', []):
        if tag in JAD_TAGS:
            topics.append(f"Jad's System/{tag}")

    # Add keyword-based topics
    for main_topic, subtopics in TOPIC_KEYWORDS.items():
        for subtopic, keywords in subtopics.items():
            for keyword in keywords:
                if keyword in text_lower:
                    topic_path = f"{main_topic}/{subtopic}"
                    if topic_path not in topics:
                        topics.append(topic_path)
                    break

    # Default to Philosophy/General if no topic found
    if not topics:
        topics.append("Philosophy/General")

    quote['topics'] = topics
    return quote

def main():
    base_path = Path("/Users/jad_irred/repos/jad/jadnohra.com/repos/77-neurons-project-perelman/export/jad/theplan")

    all_quotes = []
    quote_files = [
        "quotes1-0-100.html",
        "quotes2-104-200.html",
        "quotes3-204-313.html",
        "quotes4-314.html",
        "quotes5.html"
    ]

    for filename in quote_files:
        filepath = base_path / filename
        if filepath.exists():
            print(f"Processing {filename}...")
            with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
                html_content = f.read()
            quotes = extract_quotes_from_html(html_content)
            print(f"  Found {len(quotes)} quotes")
            all_quotes.extend(quotes)

    # Categorize all quotes
    print(f"\nCategorizing {len(all_quotes)} quotes...")
    for quote in all_quotes:
        categorize_quote(quote)

    # Build topic hierarchy
    topic_tree = {}
    for quote in all_quotes:
        for topic_path in quote['topics']:
            parts = topic_path.split('/')
            if parts[0] not in topic_tree:
                topic_tree[parts[0]] = {}
            if len(parts) > 1:
                if parts[1] not in topic_tree[parts[0]]:
                    topic_tree[parts[0]][parts[1]] = []
                topic_tree[parts[0]][parts[1]].append(len([q for q in all_quotes if topic_path in q['topics']]))

    # Output
    output = {
        'quotes': all_quotes,
        'topics': topic_tree,
        'jad_tags': JAD_TAGS,
        'stats': {
            'total_quotes': len(all_quotes),
            'topics_count': len(topic_tree)
        }
    }

    output_path = Path("/Users/jad_irred/repos/jad/jadnohra.com/docs/assets/data/quotes.json")
    output_path.parent.mkdir(parents=True, exist_ok=True)

    with open(output_path, 'w', encoding='utf-8') as f:
        json.dump(output, f, indent=2, ensure_ascii=False)

    print(f"\nOutput written to {output_path}")
    print(f"Total quotes: {len(all_quotes)}")
    print("\nTopic distribution:")
    for main_topic, subtopics in topic_tree.items():
        count = sum(len([q for q in all_quotes if any(main_topic in t for t in q['topics'])]) for _ in [1])
        print(f"  {main_topic}: {count}")

if __name__ == "__main__":
    main()
