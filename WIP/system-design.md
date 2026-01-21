# System Design

System design interviews require three layers of knowledge. 
Fundamentals explain WHY. Practicals are WHAT you just know. 
Assumptions Space is WHAT you pick from.

---

## Fundamentals → [Abstractor](/abstractor/)

The Abstractor covers WHY things work:

| Pattern | Explains |
|---------|----------|
| Derived Data | Why CDN, cache, replica, index exist |
| Three Choices | What transform, where to store, how to sync |
| Sync Obligation | Why two systems can diverge |
| Coherence Problem | Why conflicts are unavoidable |

**In interviews:** Use fundamentals to DERIVE answers, not memorize them.

---

## Practicals

Engineering knowledge you just KNOW. Not derivable from first principles.

### API Conventions
- REST: POST create, GET read, PUT update, DELETE
- Auth in headers (JWT/session), not body or URL
- `/resources/{id}/action` for actions on resources

### Security Mechanics
- HTTPS: encryption in transit
- S3 SSE: encryption at rest
- Presigned URL: signature + expiry + scoped path
- Check permissions BEFORE generating URLs

### Chunking Specifics
- 5-10MB chunk size
- SHA-256 for content hash / fingerprint
- S3 Multipart Upload API

### Protocol Specifics
- WebSocket: persistent, bidirectional
- SSE: server-push only, simpler
- Polling: client-initiated, latency = interval

### Compression Specifics  
- Text: compresses well (5:1)
- Media (jpg, mp4, zip): already compressed, skip
- Compress BEFORE encrypt

---

## Assumptions Space

Technologies you pick from. Know they exist, roughly how they work.

| Category | Technologies |
|----------|--------------|
| Blob Storage | S3, GCS, Azure Blob |
| Database | Postgres, DynamoDB, Cassandra |
| Cache | Redis, Memcached |
| CDN | CloudFront, Cloudflare |
| Queue | Kafka, SQS, RabbitMQ |
| Search | Elasticsearch |
| Auth | JWT, OAuth, presigned URLs |
| Real-time | WebSocket, SSE, polling |
| Hashing | SHA-256, MD5 |
| Compression | gzip, brotli |

---

## How They Connect
```
Interview Question
       │
       ▼
┌─────────────────────────────┐
│  FUNDAMENTALS               │
│  "Why does this problem     │
│   exist? What pattern?"     │
└──────────────┬──────────────┘
               │ derive
               ▼
┌─────────────────────────────┐
│  ASSUMPTIONS SPACE          │
│  "What techniques solve     │
│   this pattern?"            │
└──────────────┬──────────────┘
               │ pick
               ▼
┌─────────────────────────────┐
│  PRACTICALS                 │
│  "How does this technique   │
│   actually work?"           │
└─────────────────────────────┘
```

Example flow:
1. **Question:** How to handle large file uploads?
2. **Fundamental:** Long operations fail. Need granular recovery.
3. **Assumption Space:** Chunking exists. S3 Multipart exists.
4. **Practical:** 5-10MB chunks, SHA-256 identity, server-side state.

---

## Interview Quizzes

Practice the decision chain:

- [Dropbox](/system-design/dropbox/) — file storage, sync, large files
- *(more to come)*
