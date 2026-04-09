# Reverse Engineering dengan Ghidra

## Apa itu Ghidra?

Ghidra adalah **disassembler + decompiler** gratis dari NSA. Fungsinya:
- Mengubah binary (.exe) → assembly
- Mengubah assembly → pseudo C code (decompile)
- Analisis referensi, string, fungsi, dll

Download: https://ghidra-sre.org/

---

## Setup Awal

1. Install **JDK 17+** (Ghidra butuh Java)
2. Extract Ghidra, jalankan `ghidraRun.bat` (Windows)
3. Buat **Project baru**: File → New Project → Non-Shared → pilih folder

---

## Membuka Binary Pertama

### 1. Import Binary

- File → Import File → pilih `.exe` yang mau dianalisis
- Ghidra otomatis deteksi format (PE, ELF, dll)
- Klik OK, lalu double-click file di project window

### 2. Auto Analysis

Ghidra akan tanya: **"Analyze?"** → klik **Yes** dengan default options.

Ini akan:
- Mengidentifikasi semua fungsi
- Menemukan string
- Membuat cross-references (xref)
- Decompile fungsi-fungsi

Tunggu sampai progress bar selesai.

---

## Tampilan Utama Ghidra (CodeBrowser)

```
┌──────────────────────────────────────────────────────────────┐
│ Menu Bar                                                      │
├──────────┬───────────────────────────┬───────────────────────┤
│          │                           │                       │
│ Symbol   │   Listing View            │  Decompiler           │
│ Tree     │   (Assembly/Disassembly)  │  (Pseudo C Code)      │
│          │                           │                       │
│ - .text  │   mov eax, [ebp-4]       │  int main() {         │
│ - .data  │   cmp eax, 0x1e          │    int x = 10;        │
│ - main   │   jne 0x00401050         │    if (x == 30) {     │
│          │                           │                       │
├──────────┴───────────────────────────┴───────────────────────┤
│ Console / Output                                              │
└──────────────────────────────────────────────────────────────┘
```

- **Symbol Tree** (kiri): daftar fungsi, variabel, import/export
- **Listing View** (tengah): kode assembly
- **Decompiler** (kanan): pseudo C code — ini yang paling berguna!

---

## Teknik Dasar RE di Ghidra

### 1. Cari Fungsi main()

- Di Symbol Tree → cari `main` atau `_main`
- Atau: Search → For Strings → cari string yang kamu tahu ada di program
  - Double-click string → kamu dibawa ke lokasi data
  - Klik kanan string → References → Show References To
  - Ini akan menunjukkan fungsi mana yang pakai string itu

### 2. Cari String Menarik

**Search → For Strings** (atau `S` shortcut)

String yang menarik untuk RE:
- `"password"`, `"correct"`, `"wrong"`, `"access"`
- `"license"`, `"serial"`, `"key"`
- `"error"`, `"failed"`, `"success"`

### 3. Ikuti Cross-References (XREF)

Klik kanan pada fungsi/string → **References → Show References To**

Ini menunjukkan **siapa yang memanggil** fungsi/string ini. Sangat berguna untuk tracing alur program.

### 4. Rename & Re-type

Kode Ghidra awalnya penuh nama generik (`FUN_00401000`, `DAT_00403000`). Kamu harus rename agar mudah dibaca:

- Klik kanan fungsi → **Rename Function** (atau `L`)
- Klik kanan variabel di decompiler → **Rename Variable**
- Klik kanan variabel → **Retype Variable** untuk ubah tipe data

Contoh:
```
SEBELUM:
  uVar1 = FUN_00401050(param_1);
  if (uVar1 == 0) {
    FUN_00401080("Access denied");
  }

SESUDAH (setelah rename):
  result = check_password(user_input);
  if (result == 0) {
    print_message("Access denied");
  }
```

### 5. Patching di Ghidra

Klik kanan instruksi → **Patch Instruction**

Contoh: ubah `JNZ` (jump not zero) menjadi `JZ` (jump zero) untuk membalik logika.

Lalu export: File → Export Program → Binary

---

## Praktek: RE program password_check

Gunakan file `scripts/coding/password_check.c` yang sudah di-compile.

### Langkah-langkah

1. **Import** `password_check.exe` ke Ghidra
2. **Analyze** → Yes
3. **Cari string** `"Access granted"` → Search → For Strings
4. **Ikuti XREF** → double-click string → cari reference-nya
5. **Baca decompiler** → kamu akan lihat logika if-else
6. **Temukan `check_password()`** → ikuti XREF dari fungsi yang dipanggil
7. **Baca `strcmp`** → kamu akan lihat string password di decompiler!

### Yang Akan Kamu Lihat di Decompiler

```c
// Ghidra decompiler output (kurang lebih):
undefined4 check_password(char *param_1) {
    int result;
    result = strcmp(param_1, "RE2026");   // ← PASSWORD DITEMUKAN!
    if (result == 0) {
        return 1;
    }
    return 0;
}
```

### Cara Bypass Tanpa Tahu Password

1. Cari instruksi `test eax, eax` setelah `call check_password`
2. Di bawahnya pasti ada `jz` atau `jne`
3. **Patch** jump tersebut:
   - Ubah `jne` → `je` (balik logika)
   - Atau ubah `jne` → `nop nop` (hapus jump, selalu lanjut)
   - Atau ubah `test eax, eax` → `xor eax, eax` + ubah jump

---

## Shortcut Ghidra yang Penting

| Shortcut | Fungsi |
|----------|--------|
| `G` | Go to address |
| `L` | Rename / Label |
| `T` | Re-type |
| `X` | Show references (XREF) |
| `S` | Search strings |
| `Ctrl+Shift+E` | Search for bytes/instruction |
| `Space` | Toggle graph/listing view |
| `Esc` | Go back (navigasi mundur) |
| `Alt+←` | Go back |
| `Alt+→` | Go forward |

---

## Tips RE untuk Pemula

1. **Mulai dari string** — cari string error/success, lalu trace ke belakang
2. **Decompiler > Assembly** — baca decompiler dulu, assembly kalau perlu detail
3. **Rename semuanya** — jangan biarkan nama generik, rename agar mudah dibaca
4. **Ikuti XREF** — ini senjata utama untuk navigasi di binary besar
5. **Bandingkan input/output** — jalankan program dulu, pahami behavior, baru bongkar
