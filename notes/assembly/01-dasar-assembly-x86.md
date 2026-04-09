# Dasar Assembly x86

Assembly adalah bahasa paling dekat dengan mesin. Semua program yang dibongkar dalam RE akan tampil dalam bentuk assembly.

## Apa itu Assembly?

Komputer cuma ngerti angka (binary). Assembly adalah representasi manusia dari instruksi mesin.

```
Source code (C)          Assembly              Machine code
─────────────           ─────────             ─────────────
int a = 5 + 3;    →    mov eax, 5       →    B8 05 00 00 00
                        add eax, 3       →    83 C0 03
```

---

## Register — "Variabel" di CPU

Register = tempat penyimpanan super cepat langsung di dalam CPU.

### Register Umum (32-bit)

| Register | Nama | Fungsi Umum |
|----------|------|-------------|
| **EAX** | Accumulator | Hasil kalkulasi, return value fungsi |
| **EBX** | Base | Penyimpanan umum |
| **ECX** | Counter | Counter untuk loop |
| **EDX** | Data | Data tambahan, aritmatika |
| **ESP** | Stack Pointer | Menunjuk puncak stack |
| **EBP** | Base Pointer | Menunjuk dasar stack frame |
| **ESI** | Source Index | Sumber saat copy data |
| **EDI** | Destination Index | Tujuan saat copy data |
| **EIP** | Instruction Pointer | Alamat instruksi yang sedang dieksekusi |

> EIP adalah yang paling penting di RE — kalau kamu bisa kontrol EIP, kamu kontrol program.

### Ukuran Register

```
EAX  = 32 bit (seluruh)     → 11223344
 AX  = 16 bit (bawah)       →     3344
 AH  = 8 bit (atas dari AX) →     33
 AL  = 8 bit (bawah dari AX)→       44

|---- EAX (32-bit) ---------|
              |---- AX (16) -|
              | AH  |  AL    |
```

---

## Instruksi Dasar

### MOV — Pindahkan data
```asm
mov eax, 10        ; eax = 10
mov ebx, eax       ; ebx = eax (copy nilai)
mov ecx, [eax]     ; ecx = nilai di ALAMAT yang ditunjuk eax (dereference pointer)
```

> Tanda `[ ]` artinya ambil nilai dari alamat memori itu (seperti *pointer di C).

### ADD / SUB — Tambah / Kurang
```asm
add eax, 5         ; eax = eax + 5
sub ebx, 3         ; ebx = ebx - 3
```

### INC / DEC — Tambah 1 / Kurang 1
```asm
inc ecx            ; ecx++ 
dec ecx            ; ecx--
```

### CMP — Bandingkan
```asm
cmp eax, 10        ; bandingkan eax dengan 10 (set flags, tidak simpan hasil)
```

### Jump — Lompat
```asm
jmp label          ; lompat tanpa syarat
je  label          ; jump if equal (kalau sama)
jne label          ; jump if not equal (kalau beda)
jg  label          ; jump if greater
jl  label          ; jump if less
```

### Contoh: If-Else
```c
// C code
if (a == 5) {
    b = 1;
} else {
    b = 0;
}
```
```asm
; Assembly
    cmp eax, 5
    jne else_branch
    mov ebx, 1
    jmp done
else_branch:
    mov ebx, 0
done:
```

---

## Stack — Tumpukan Data

Stack bekerja seperti tumpukan piring — yang terakhir ditaruh, yang pertama diambil (LIFO).

```
Alamat tinggi
┌─────────────┐
│   ...       │
├─────────────┤
│  var lokal  │ ← EBP - 4
├─────────────┤
│  old EBP    │ ← EBP
├─────────────┤
│  return addr│ ← alamat balik setelah fungsi selesai
├─────────────┤
│  argument   │
├─────────────┤
│   ...       │ ← ESP (puncak stack)
└─────────────┘
Alamat rendah
```

### PUSH / POP
```asm
push eax           ; taruh eax ke stack, ESP turun
pop ebx            ; ambil dari stack ke ebx, ESP naik
```

### CALL / RET
```asm
call my_function   ; push return address, lompat ke fungsi
ret                ; pop return address, lompat balik
```

---

## Pola Fungsi (Prologue & Epilogue)

Pola ini muncul di SETIAP fungsi saat reverse engineering:

```asm
my_function:
    push ebp           ; simpan base pointer lama      ← PROLOGUE
    mov ebp, esp       ; set base pointer baru          ←
    sub esp, 16        ; alokasi variabel lokal         ←
    
    ; ... isi fungsi ...
    
    mov esp, ebp       ; bersihkan variabel lokal       ← EPILOGUE
    pop ebp            ; kembalikan base pointer lama    ←
    ret                ; kembali ke pemanggil            ←
```
