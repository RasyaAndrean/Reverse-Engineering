# Calling Conventions, Flags Register & Addressing Modes

## 1. Calling Conventions

Calling convention = aturan bagaimana fungsi dipanggil: bagaimana argumen dikirim, siapa yang bersihkan stack, register mana yang harus dijaga.

### cdecl (C Declaration) — Paling umum di x86

```
Aturan:
- Argumen di-push ke stack dari KANAN ke KIRI
- CALLER yang bersihkan stack (add esp, N)
- Return value di EAX
```

```asm
; memanggil: result = tambah(10, 20)
push 20            ; argument kedua dulu (kanan)
push 10            ; argument pertama (kiri)
call tambah
add esp, 8         ; CALLER bersihkan 2 argumen × 4 byte = 8
mov [result], eax  ; simpan return value
```

### stdcall — Dipakai Windows API

```
Aturan:
- Argumen di-push dari KANAN ke KIRI (sama seperti cdecl)
- CALLEE (fungsi) yang bersihkan stack (ret N)
- Return value di EAX
```

```asm
; memanggil: MessageBoxA(0, "hi", "title", 0)
push 0
push offset title
push offset text
push 0
call MessageBoxA   ; fungsi bersihkan sendiri stack-nya
; TIDAK perlu add esp — sudah dibersihkan oleh MessageBoxA
```

Di dalam fungsi stdcall:
```asm
MessageBoxA:
    ; ... isi fungsi ...
    ret 16         ; return + pop 16 byte (4 argumen × 4 byte)
```

### fastcall — Argumen via Register

```
Aturan:
- 2 argumen pertama via ECX dan EDX
- Sisanya di stack
- CALLEE bersihkan stack
```

### Perbandingan

| | cdecl | stdcall | fastcall |
|--|-------|---------|----------|
| Argumen | Stack | Stack | ECX, EDX, lalu stack |
| Bersihkan stack | Caller | Callee | Callee |
| Variadic (...) | Ya | Tidak | Tidak |
| Dipakai di | Linux, umum | Windows API | Optimasi |

> **Tips RE:** Kalau lihat `add esp, N` setelah `call`, itu **cdecl**. Kalau tidak ada, kemungkinan **stdcall** atau **fastcall**.

---

## 2. Flags Register (EFLAGS)

Flags adalah bit-bit di CPU yang menyimpan hasil operasi terakhir. Instruksi `cmp`, `test`, `add`, `sub` mengubah flags.

### Flags yang Paling Penting untuk RE

| Flag | Nama | Kapan di-set (=1) |
|------|------|-------------------|
| **ZF** | Zero Flag | Hasil operasi = 0 |
| **SF** | Sign Flag | Hasil operasi negatif (bit tertinggi = 1) |
| **CF** | Carry Flag | Ada carry/borrow (unsigned overflow) |
| **OF** | Overflow Flag | Signed overflow |

### Bagaimana CMP Bekerja

`cmp a, b` secara internal melakukan `a - b` tanpa menyimpan hasil, hanya set flags.

```
cmp eax, ebx    ; hitung eax - ebx, set flags

Contoh:
  eax = 5, ebx = 5  →  5 - 5 = 0  →  ZF=1, SF=0
  eax = 3, ebx = 5  →  3 - 5 = -2 →  ZF=0, SF=1
  eax = 7, ebx = 5  →  7 - 5 = 2  →  ZF=0, SF=0
```

### Jump Berdasarkan Flags

| Instruksi | Kondisi | Flags yang Dicek |
|-----------|---------|-----------------|
| `je` / `jz` | Equal / Zero | ZF = 1 |
| `jne` / `jnz` | Not Equal / Not Zero | ZF = 0 |
| `jg` | Greater (signed) | ZF=0 dan SF=OF |
| `jl` | Less (signed) | SF != OF |
| `ja` | Above (unsigned) | CF=0 dan ZF=0 |
| `jb` | Below (unsigned) | CF = 1 |

### TEST vs CMP

```asm
test eax, eax   ; AND eax dengan dirinya sendiri — cek apakah eax = 0
jz is_zero       ; kalau ZF=1 (eax=0), lompat
```

> **Pola RE:** `test eax, eax` + `jz/jnz` adalah cara cek return value fungsi.
> Kalau fungsi return 0 (gagal) → ZF=1 → `jz` akan lompat.

---

## 3. Addressing Modes

Cara instruksi mengakses data di memori.

### Immediate — Nilai langsung
```asm
mov eax, 42         ; eax = 42
```

### Register — Dari register lain
```asm
mov eax, ebx        ; eax = ebx
```

### Direct — Dari alamat tetap
```asm
mov eax, [0x00401000]  ; eax = nilai di alamat 0x00401000
```

### Register Indirect — Alamat dari register
```asm
mov eax, [ebx]         ; eax = nilai di alamat yang ada di ebx
```

Seperti pointer di C: `eax = *ebx`

### Base + Offset — Akses struct/variabel lokal
```asm
mov eax, [ebp-4]       ; variabel lokal pertama
mov eax, [ebp+8]       ; parameter pertama fungsi
mov eax, [ebx+16]      ; seperti struct: field di offset 16
```

Seperti C: `eax = obj->field` dimana field ada di offset 16

### Base + Index × Scale + Offset — Akses array
```asm
mov eax, [ebx + ecx*4]       ; array[i] dimana tiap elemen 4 byte
mov eax, [ebx + ecx*4 + 8]   ; struct.array[i]
```

Formula lengkap: `[base + index × scale + displacement]`
- **scale** bisa 1, 2, 4, atau 8
- Ini sering muncul saat akses array

Contoh di C:
```c
int arr[10];
int x = arr[i];
// Assembly: mov eax, [ebx + ecx*4]
//   ebx = base address arr
//   ecx = index i
//   4 = sizeof(int)
```

### LEA — Load Effective Address

```asm
lea eax, [ebx + ecx*4 + 8]   ; eax = ebx + ecx*4 + 8 (HITUNG alamat, bukan ambil nilai)
```

> **Perbedaan MOV vs LEA:**
> - `mov eax, [ebx+4]` → ambil **nilai** di alamat ebx+4
> - `lea eax, [ebx+4]` → simpan **alamat** ebx+4 ke eax (tidak akses memori)

LEA sering dipakai compiler untuk aritmatika cepat:
```asm
lea eax, [eax + eax*2]   ; eax = eax × 3 (tanpa instruksi mul!)
```

---

## Cheat Sheet: Pola yang Sering Muncul di RE

```asm
; Cek return value fungsi
call some_func
test eax, eax
jz failed            ; kalau return 0, lompat ke gagal

; Loop
mov ecx, 10          ; counter = 10
.loop:
    ; ... isi loop ...
    dec ecx
    jnz .loop        ; kalau ecx != 0, ulangi

; Akses array dalam loop
xor ecx, ecx         ; i = 0
.loop:
    mov eax, [ebx + ecx*4]  ; eax = array[i]
    ; ... proses ...
    inc ecx
    cmp ecx, 10
    jl .loop

; Switch/jump table
mov eax, [input]
cmp eax, 4
ja default_case
jmp [jump_table + eax*4]   ; lompat berdasarkan nilai input
```
