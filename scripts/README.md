# Encrypted Bio-Extended Page

The `/bio-extended/` page is a self-decrypting HTML page. The plaintext content lives in `docs/bio-extended-content.md` (gitignored, never committed). The encrypted output `docs/bio-extended.html` is what gets committed and deployed.

## How to update

1. Edit `docs/bio-extended-content.md` with your content (markdown)
2. Run:
   ```
   ./scripts/update-bio.sh
   ```
3. Enter your password when prompted
4. Commit and push:
   ```
   git add docs/bio-extended.html
   git commit -m "Update bio-extended"
   git push
   ```

## How it works

- Content is encrypted with AES-256-GCM (Web Crypto API compatible)
- Key derived via PBKDF2 with 600,000 iterations and SHA-256
- The generated HTML contains only the encrypted ciphertext, salt, and IV
- Decryption happens entirely client-side in the browser using Web Crypto API
- A correct password is remembered in `sessionStorage` for same-session revisits
- Wrong passwords show an error with a shake animation

## Files

- `scripts/encrypt-page.sh` — the encryption script (uses Python3 + libcrypto)
- `scripts/update-bio.sh` — convenience wrapper that calls encrypt-page.sh with the right paths
- `docs/bio-extended-content.md` — your plaintext content (**gitignored**)
- `docs/bio-extended.html` — encrypted output (committed)
