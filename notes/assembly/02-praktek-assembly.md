# Praktek Assembly — Tulis & Debug

## Program 1: Hello Calculator

Kita buat program C sederhana lalu lihat assembly-nya.

### Source Code (C)

Simpan sebagai `calc.c`:

```c
#include <stdio.h>

int tambah(int a, int b) {
    int hasil = a + b;
    return hasil;
}

int main() {
    int x = 10;
    int y = 20;
    int z = tambah(x, y);
    
    if (z == 30) {
        printf("Benar! z = %d\n", z);
    } else {
        printf("Salah! z = %d\n", z);
    }
    return 0;
}
```

### Compile & Lihat Assembly

```bash
# Compile ke assembly (output: calc.s)
gcc -S -m32 -masm=intel -O0 calc.c -o calc.s

# Compile ke executable
gcc -m32 -O0 -g calc.c -o calc.exe
```

Flag penting:
- `-S` → output assembly, bukan binary
- `-m32` → compile 32-bit (x86)
- `-masm=intel` → syntax Intel (lebih mudah dibaca)
- `-O0` → tanpa optimasi (assembly cocok dengan source)
- `-g` → include debug info

### Hasil Assembly (yang penting)

```asm
_tambah:
    push ebp                ; prologue
    mov ebp, esp
    sub esp, 16
    
    mov edx, [ebp+8]       ; ambil parameter a
    mov eax, [ebp+12]      ; ambil parameter b
    add eax, edx           ; eax = a + b
    mov [ebp-4], eax       ; simpan ke variabel lokal 'hasil'
    mov eax, [ebp-4]       ; return value di eax
    
    leave                   ; epilogue (= mov esp,ebp + pop ebp)
    ret

_main:
    push ebp
    mov ebp, esp
    sub esp, 32
    
    mov DWORD [ebp-4], 10  ; x = 10
    mov DWORD [ebp-8], 20  ; y = 20
    
    push DWORD [ebp-8]     ; push y (argument kedua)
    push DWORD [ebp-4]     ; push x (argument pertama)
    call _tambah            ; panggil tambah(x, y)
    add esp, 8             ; bersihkan arguments dari stack
    mov [ebp-12], eax      ; z = return value
    
    cmp DWORD [ebp-12], 30 ; if (z == 30)
    jne .L_else            ; kalau beda, lompat ke else
    ; ... printf("Benar!") ...
    jmp .L_done
.L_else:
    ; ... printf("Salah!") ...
.L_done:
    mov eax, 0             ; return 0
    leave
    ret
```

### Pelajaran dari Assembly Ini

1. **Parameter di-push ke stack secara terbalik** (y dulu, baru x)
2. **Return value selalu di EAX**
3. **Variabel lokal diakses via `[ebp-N]`**
4. **Argumen fungsi diakses via `[ebp+N]`**
5. **`cmp` + `jne`** = pola if-else

---

## Debug dengan x64dbg

### Langkah-langkah

1. **Buka x64dbg** → File → Open → pilih `calc.exe`
2. Program berhenti di **entry point** (bukan main)

### Navigasi ke main()
- Klik kanan di CPU view → **Search for** → **All intermodular calls**
- Cari `printf` atau `main`
- Atau tekan `Ctrl+G` → ketik nama fungsi

### Perintah Penting

| Tombol | Fungsi |
|--------|--------|
| **F7** | Step Into — masuk ke dalam fungsi |
| **F8** | Step Over — jalankan fungsi tanpa masuk |
| **F9** | Run — jalankan sampai breakpoint berikutnya |
| **F2** | Toggle Breakpoint — pasang/hapus breakpoint |
| **Space** | Edit instruksi (patching) |

### Apa yang Harus Diperhatikan

1. **Register panel** (kanan atas) — perhatikan EAX, ESP, EBP berubah
2. **Stack panel** (kanan bawah) — lihat push/pop terjadi
3. **Flags** — perhatikan ZF (Zero Flag) berubah setelah `cmp`

### Eksperimen: Paksa Program ke Branch Lain

1. Jalankan sampai instruksi `cmp DWORD [ebp-12], 30`
2. Lihat hasilnya (ZF = 1 karena sama)
3. Klik kanan pada `jne` → **Set EIP here** atau ubah ZF secara manual
4. Sekarang program akan masuk ke branch yang berbeda!

> Ini adalah dasar dari **cracking** — mengubah flow program dengan memodifikasi jump atau flag.

---

## Program 2: Password Check (untuk latihan RE)

```c
#include <stdio.h>
#include <string.h>

int check_password(char *input) {
    char *secret = "RE2026";
    if (strcmp(input, secret) == 0) {
        return 1;
    }
    return 0;
}

int main() {
    char buffer[64];
    printf("Enter password: ");
    scanf("%63s", buffer);
    
    if (check_password(buffer)) {
        printf("Access granted!\n");
    } else {
        printf("Access denied!\n");
    }
    return 0;
}
```

### Tantangan RE

Compile program ini, lalu coba:

1. **Buka di x64dbg** → cari fungsi `check_password`
2. **Temukan string "RE2026"** → klik kanan → Search for → All referenced strings
3. **Bypass check** → ubah `jne` (jump not equal) menjadi `je` (jump equal) atau `nop`
4. **Atau** → set EAX = 1 sebelum `ret` dari check_password

Ini simulasi dasar reverse engineering yang sebenarnya!
