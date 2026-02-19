#!/usr/bin/env bash
set -euo pipefail

usage() {
  echo "Usage: $0 <input-file> <output-file> [password]"
  echo ""
  echo "Encrypts a markdown/HTML content file into a self-decrypting HTML page."
  echo "If password is not provided, you will be prompted for it."
  echo ""
  echo "Example:"
  echo "  $0 docs/bio-ext-content.md docs/bio-ext.html"
  exit 1
}

if [ $# -lt 2 ]; then
  usage
fi

INPUT_FILE="$1"
OUTPUT_FILE="$2"

if [ ! -f "$INPUT_FILE" ]; then
  echo "Error: Input file '$INPUT_FILE' not found."
  exit 1
fi

if [ $# -ge 3 ]; then
  PASSWORD="$3"
else
  read -rsp "Enter encryption password: " PASSWORD
  echo
  read -rsp "Confirm password: " PASSWORD_CONFIRM
  echo
  if [ "$PASSWORD" != "$PASSWORD_CONFIRM" ]; then
    echo "Error: Passwords do not match."
    exit 1
  fi
fi

if [ -z "$PASSWORD" ]; then
  echo "Error: Password cannot be empty."
  exit 1
fi

# Do all crypto in Python for reliability
CRYPTO_OUTPUT=$(ENC_PASSWORD="$PASSWORD" ENC_INPUT="$INPUT_FILE" python3 << 'PYEOF'
import hashlib, base64, os, sys, json

# Read inputs from environment
password = os.environ['ENC_PASSWORD']
input_file = os.environ['ENC_INPUT']

with open(input_file, 'r') as f:
    plaintext = f.read()

# Generate random salt (16 bytes) and IV (12 bytes)
salt = os.urandom(16)
iv = os.urandom(12)

# Derive key using PBKDF2: 600000 iterations, SHA-256, 32-byte key
key = hashlib.pbkdf2_hmac('sha256', password.encode('utf-8'), salt, 600000, dklen=32)

# AES-256-GCM encryption via ctypes + libcrypto
import ctypes, ctypes.util

libcrypto_path = ctypes.util.find_library('crypto')
if not libcrypto_path:
    for p in ['/opt/homebrew/lib/libcrypto.dylib', '/usr/lib/libcrypto.dylib', '/usr/local/lib/libcrypto.dylib']:
        if os.path.exists(p):
            libcrypto_path = p
            break

if not libcrypto_path:
    print("ERROR: libcrypto not found", file=sys.stderr)
    sys.exit(1)

libcrypto = ctypes.CDLL(libcrypto_path)

# Set up function signatures
EVP_CIPHER_CTX_new = libcrypto.EVP_CIPHER_CTX_new
EVP_CIPHER_CTX_new.restype = ctypes.c_void_p

EVP_CIPHER_CTX_free = libcrypto.EVP_CIPHER_CTX_free
EVP_CIPHER_CTX_free.argtypes = [ctypes.c_void_p]

EVP_EncryptInit_ex = libcrypto.EVP_EncryptInit_ex
EVP_EncryptInit_ex.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p,
                                ctypes.c_char_p, ctypes.c_char_p]
EVP_EncryptInit_ex.restype = ctypes.c_int

EVP_EncryptUpdate = libcrypto.EVP_EncryptUpdate
EVP_EncryptUpdate.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.POINTER(ctypes.c_int),
                               ctypes.c_char_p, ctypes.c_int]
EVP_EncryptUpdate.restype = ctypes.c_int

EVP_EncryptFinal_ex = libcrypto.EVP_EncryptFinal_ex
EVP_EncryptFinal_ex.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.POINTER(ctypes.c_int)]
EVP_EncryptFinal_ex.restype = ctypes.c_int

EVP_CIPHER_CTX_ctrl = libcrypto.EVP_CIPHER_CTX_ctrl
EVP_CIPHER_CTX_ctrl.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.c_char_p]
EVP_CIPHER_CTX_ctrl.restype = ctypes.c_int

EVP_aes_256_gcm = libcrypto.EVP_aes_256_gcm
EVP_aes_256_gcm.restype = ctypes.c_void_p

EVP_CTRL_GCM_SET_IVLEN = 0x9
EVP_CTRL_GCM_GET_TAG = 0x10

plaintext_bytes = plaintext.encode('utf-8')

ctx = EVP_CIPHER_CTX_new()
cipher = EVP_aes_256_gcm()

EVP_EncryptInit_ex(ctx, cipher, None, None, None)
EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, len(iv), None)
EVP_EncryptInit_ex(ctx, None, None, key, iv)

outbuf = ctypes.create_string_buffer(len(plaintext_bytes) + 16)
outlen = ctypes.c_int(0)
EVP_EncryptUpdate(ctx, outbuf, ctypes.byref(outlen), plaintext_bytes, len(plaintext_bytes))
ciphertext = outbuf.raw[:outlen.value]

finalbuf = ctypes.create_string_buffer(16)
finallen = ctypes.c_int(0)
EVP_EncryptFinal_ex(ctx, finalbuf, ctypes.byref(finallen))
ciphertext += finalbuf.raw[:finallen.value]

tag = ctypes.create_string_buffer(16)
EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag)

EVP_CIPHER_CTX_free(ctx)

# AES-GCM format: ciphertext + tag (matches Web Crypto API)
encrypted = base64.b64encode(ciphertext + tag.raw).decode()

# Output as JSON for safe parsing
print(json.dumps({
    'salt': salt.hex(),
    'iv': iv.hex(),
    'data': encrypted
}))
PYEOF
)

if [ -z "$CRYPTO_OUTPUT" ]; then
  echo "Error: Encryption failed."
  exit 1
fi

# Parse the JSON output
SALT=$(echo "$CRYPTO_OUTPUT" | python3 -c "import json,sys; d=json.load(sys.stdin); print(d['salt'])")
IV=$(echo "$CRYPTO_OUTPUT" | python3 -c "import json,sys; d=json.load(sys.stdin); print(d['iv'])")
ENCRYPTED=$(echo "$CRYPTO_OUTPUT" | python3 -c "import json,sys; d=json.load(sys.stdin); print(d['data'])")

# Write the self-decrypting HTML page
cat > "$OUTPUT_FILE" << 'HTMLEOF'
---
layout: none
permalink: /bio-ext/
sitemap: false
---
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Bio - Jad Nohra</title>
<meta name="robots" content="noindex, nofollow">
<style>
  *{margin:0;padding:0;box-sizing:border-box}
  body{
    font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,Oxygen,Ubuntu,sans-serif;
    background:#1b1b1e;color:#d1d1d6;min-height:100vh;
    display:flex;align-items:center;justify-content:center;
  }
  #unlock-screen{
    text-align:center;padding:2rem;max-width:420px;width:100%;
  }
  #unlock-screen h1{
    font-size:1.5rem;font-weight:600;color:#e5e5ea;margin-bottom:.5rem;
  }
  #unlock-screen p{
    font-size:.9rem;color:#8e8e93;margin-bottom:1.5rem;
  }
  .input-group{
    display:flex;gap:.5rem;margin-bottom:.75rem;
  }
  #password-input{
    flex:1;padding:.65rem 1rem;border-radius:8px;border:1px solid #3a3a3c;
    background:#2c2c2e;color:#e5e5ea;font-size:1rem;outline:none;
    transition:border-color .2s;
  }
  #password-input:focus{border-color:#0a84ff}
  #unlock-btn{
    padding:.65rem 1.25rem;border-radius:8px;border:none;
    background:#0a84ff;color:#fff;font-size:1rem;font-weight:500;
    cursor:pointer;transition:background .2s;white-space:nowrap;
  }
  #unlock-btn:hover{background:#0070e0}
  #unlock-btn:active{background:#005bb5}
  #error-msg{
    color:#ff453a;font-size:.85rem;min-height:1.2em;
    transition:opacity .2s;opacity:0;
  }
  #error-msg.visible{opacity:1}
  @keyframes shake{
    0%,100%{transform:translateX(0)}
    20%,60%{transform:translateX(-8px)}
    40%,80%{transform:translateX(8px)}
  }
  .shake{animation:shake .4s ease-in-out}

  /* Decrypted content styles */
  #content{
    display:none;max-width:800px;margin:2rem auto;padding:1.5rem 2rem;
    line-height:1.7;font-size:1.05rem;
  }
  #content h1,#content h2,#content h3{color:#e5e5ea;margin:1.5rem 0 .75rem;font-weight:600}
  #content h1{font-size:1.8rem;border-bottom:1px solid #3a3a3c;padding-bottom:.5rem}
  #content h2{font-size:1.4rem}
  #content h3{font-size:1.15rem}
  #content p{margin-bottom:1rem}
  #content a{color:#0a84ff;text-decoration:none}
  #content a:hover{text-decoration:underline}
  #content ul,#content ol{margin:0 0 1rem 1.5rem}
  #content li{margin-bottom:.35rem}
  #content blockquote{
    border-left:3px solid #0a84ff;padding:.5rem 1rem;margin:1rem 0;
    color:#a1a1a6;background:#2c2c2e;border-radius:0 8px 8px 0;
  }
  #content code{
    background:#2c2c2e;padding:.15rem .4rem;border-radius:4px;font-size:.9em;
  }
  #content pre{
    background:#2c2c2e;padding:1rem;border-radius:8px;overflow-x:auto;margin:1rem 0;
  }
  #content pre code{background:none;padding:0}
  #content strong{color:#e5e5ea}
  #content hr{border:none;border-top:1px solid #3a3a3c;margin:2rem 0}
</style>
</head>
<body>

<div id="unlock-screen">
  <h1>Protected Page</h1>
  <p>This content is encrypted. Enter the password to view.</p>
  <div class="input-group">
    <input type="password" id="password-input" placeholder="Password" autocomplete="off" autofocus>
    <button id="unlock-btn">Unlock</button>
  </div>
  <div id="error-msg">&nbsp;</div>
</div>

<div id="content"></div>

<script>
ENCRYPTED_DATA
</script>

<script>
(function() {
  const SESSION_KEY = 'bio-ext-pw';

  function hexToBytes(hex) {
    const bytes = new Uint8Array(hex.length / 2);
    for (let i = 0; i < hex.length; i += 2)
      bytes[i / 2] = parseInt(hex.substr(i, 2), 16);
    return bytes;
  }

  async function decrypt(password) {
    const enc = new TextEncoder();
    const passwordKey = await crypto.subtle.importKey(
      'raw', enc.encode(password), 'PBKDF2', false, ['deriveKey']
    );
    const salt = hexToBytes(window.__encSalt);
    const iv = hexToBytes(window.__encIV);
    const ciphertext = Uint8Array.from(atob(window.__encData), c => c.charCodeAt(0));

    const key = await crypto.subtle.deriveKey(
      { name: 'PBKDF2', salt: salt, iterations: 600000, hash: 'SHA-256' },
      passwordKey,
      { name: 'AES-GCM', length: 256 },
      false,
      ['decrypt']
    );

    const decrypted = await crypto.subtle.decrypt(
      { name: 'AES-GCM', iv: iv },
      key,
      ciphertext
    );

    return new TextDecoder().decode(decrypted);
  }

  // Simple markdown-to-HTML (handles common patterns)
  function renderMarkdown(md) {
    // Strip YAML front matter
    md = md.replace(/^---[\s\S]*?---\s*/, '');

    let html = md
      // Code blocks
      .replace(/```(\w*)\n([\s\S]*?)```/g, '<pre><code>$2</code></pre>')
      // Headings
      .replace(/^### (.+)$/gm, '<h3>$1</h3>')
      .replace(/^## (.+)$/gm, '<h2>$1</h2>')
      .replace(/^# (.+)$/gm, '<h1>$1</h1>')
      // Bold and italic
      .replace(/\*\*\*(.+?)\*\*\*/g, '<strong><em>$1</em></strong>')
      .replace(/\*\*(.+?)\*\*/g, '<strong>$1</strong>')
      .replace(/\*(.+?)\*/g, '<em>$1</em>')
      // Links
      .replace(/\[([^\]]+)\]\(([^)]+)\)/g, '<a href="$2" target="_blank" rel="noopener">$1</a>')
      // Horizontal rules
      .replace(/^---$/gm, '<hr>')
      .replace(/^\*\*\*$/gm, '<hr>')
      // Blockquotes
      .replace(/^> (.+)$/gm, '<blockquote>$1</blockquote>')
      // Inline code
      .replace(/`([^`]+)`/g, '<code>$1</code>')
      // Unordered lists
      .replace(/^[*-] (.+)$/gm, '<li>$1</li>')
      // Paragraphs - wrap remaining lines
      .replace(/^(?!<[hlpuob]|<li|<pre|<code|<hr|<block)(.*\S.*)$/gm, '<p>$1</p>');

    // Wrap consecutive <li> in <ul>
    html = html.replace(/((?:<li>.*<\/li>\s*)+)/g, '<ul>$1</ul>');

    return html;
  }

  async function tryDecrypt(password) {
    try {
      const plaintext = await decrypt(password);
      const trimmed = plaintext.trim();
      if (trimmed.startsWith('<!') || trimmed.startsWith('<html')) {
        // Full HTML document — replace entire page
        document.open();
        document.write(plaintext);
        document.close();
      } else if (trimmed.startsWith('<')) {
        // HTML fragment — inject into content div
        document.getElementById('unlock-screen').style.display = 'none';
        const contentEl = document.getElementById('content');
        contentEl.innerHTML = trimmed;
        contentEl.style.display = 'block';
        document.body.style.alignItems = 'flex-start';
      } else {
        // Markdown — render then inject
        document.getElementById('unlock-screen').style.display = 'none';
        const contentEl = document.getElementById('content');
        contentEl.innerHTML = renderMarkdown(plaintext);
        contentEl.style.display = 'block';
        document.body.style.alignItems = 'flex-start';
      }
      sessionStorage.setItem(SESSION_KEY, password);
      return true;
    } catch (e) {
      return false;
    }
  }

  // Check sessionStorage for cached password
  const cached = sessionStorage.getItem(SESSION_KEY);
  if (cached) {
    tryDecrypt(cached);
  }

  document.getElementById('unlock-btn').addEventListener('click', async function() {
    const pw = document.getElementById('password-input').value;
    if (!pw) return;
    this.disabled = true;
    this.textContent = '...';
    const ok = await tryDecrypt(pw);
    if (!ok) {
      const errEl = document.getElementById('error-msg');
      errEl.textContent = 'Wrong password. Please try again.';
      errEl.classList.add('visible');
      const input = document.getElementById('password-input');
      input.classList.add('shake');
      setTimeout(() => input.classList.remove('shake'), 400);
      input.value = '';
      input.focus();
    }
    this.disabled = false;
    this.textContent = 'Unlock';
  });

  document.getElementById('password-input').addEventListener('keydown', function(e) {
    if (e.key === 'Enter') document.getElementById('unlock-btn').click();
  });
})();
</script>

</body>
</html>
HTMLEOF

# Inject the encrypted data into the HTML
sed -i.bak "s|ENCRYPTED_DATA|window.__encSalt='${SALT}';window.__encIV='${IV}';window.__encData='${ENCRYPTED}';|" "$OUTPUT_FILE"
rm -f "${OUTPUT_FILE}.bak"

# Verify no plaintext leaked — find a content-specific line to check against
# Skip blank lines, HTML boilerplate, and short lines
CHECK_STR=$(grep -vE '^\s*$|^\s*<[!?/]|^\s*<(html|head|body|meta|link|script|style|div|span|p|h[1-6]|ul|ol|li|a |img |br|hr)|^\s*[{}]' "$INPUT_FILE" | head -1 | cut -c1-40)
if [ -n "$CHECK_STR" ] && [ ${#CHECK_STR} -gt 10 ]; then
  if grep -qF "$CHECK_STR" "$OUTPUT_FILE" 2>/dev/null; then
    echo "WARNING: Plaintext may have leaked into output!"
    rm -f "$OUTPUT_FILE"
    exit 1
  fi
fi

echo "Encrypted '$INPUT_FILE' -> '$OUTPUT_FILE'"
echo "Salt: ${SALT:0:8}... IV: ${IV:0:8}..."
echo "Output size: $(wc -c < "$OUTPUT_FILE" | tr -d ' ') bytes"
