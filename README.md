<div align="center">

# Reverse Engineering Journey

**Perjalanan pribadi mempelajari rekayasa balik — dari nol sampai paham.**

Repo ini mendokumentasikan setiap langkah belajar reverse engineering: catatan, eksperimen, tools, dan code yang saya tulis sepanjang jalan.

[![GitHub last commit](https://img.shields.io/github/last-commit/RasyaAndrean/Reverse-Engineering?style=flat-square)](https://github.com/RasyaAndrean/Reverse-Engineering/commits/main)
[![GitHub repo size](https://img.shields.io/github/repo-size/RasyaAndrean/Reverse-Engineering?style=flat-square)](https://github.com/RasyaAndrean/Reverse-Engineering)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square)](LICENSE)

---

*"The best way to understand how something works is to take it apart."*

</div>

## Tentang Repo Ini

Saya mulai belajar reverse engineering dari nol. Repo ini adalah **jejak perjalanan** saya — setiap push adalah bukti bahwa saya sudah selangkah lebih maju. Semua catatan, code, dan eksperimen saya simpan di sini supaya:

- Saya punya **catatan yang bisa dilihat kembali** kapan saja
- Siapa pun yang menemukan repo ini bisa **ikut belajar** dari pengalaman saya
- Menjadi **portofolio** perjalanan belajar yang nyata

> [!NOTE]
> Repo ini terus berkembang seiring saya belajar. Kalau kamu menemukan kesalahan atau punya saran, silakan buka issue atau pull request.

---

## Struktur Repo

```
Reverse-Engineering/
│
├── diary/                    Catatan harian — apa yang dipelajari setiap push
│   └── DD-MM-YYYY/          Subfolder per tanggal push
│       ├── catatan-1.md     Catatan pertama hari itu
│       ├── catatan-2.md     Catatan kedua, dst.
│       └── ...
│
├── tools/                    Alat-alat RE dan cara penggunaannya
│   ├── disassemblers/        Ghidra, IDA, Binary Ninja
│   ├── debuggers/            x64dbg, WinDbg, GDB
│   ├── monitors/             Process Monitor, API Monitor
│   └── utilities/            HxD, PE-bear, CFF Explorer
│
├── techniques/               Teknik-teknik reverse engineering
│   ├── static-analysis/      Analisis tanpa menjalankan program
│   ├── dynamic-analysis/     Analisis sambil menjalankan program
│   └── anti-re/              Anti-RE & cara bypass-nya
│
├── experiments/              Eksperimen langsung dengan binary
│   ├── crackmes/             Latihan crackme
│   ├── malware-analysis/     Analisis malware (sample aman)
│   └── ctf/                  Challenge CTF
│
├── notes/                    Catatan teori & konsep dasar
│   ├── assembly/             Bahasa assembly (x86, x64, ARM)
│   ├── pe-format/            Struktur file PE (Windows)
│   ├── elf-format/           Struktur file ELF (Linux)
│   └── os-internals/         Konsep OS (memory, process, dll)
│
└── scripts/                  Script, tools & code hasil belajar
    └── coding/               Code contoh & latihan
```

---

## Tools yang Digunakan

| Kategori | Tools | Keterangan |
|:---------|:------|:-----------|
| **Disassembler** | Ghidra, IDA Free | Mengubah binary → assembly / pseudo-C |
| **Debugger** | x64dbg, WinDbg | Jalankan program step-by-step, inspeksi memori |
| **Hex Editor** | HxD, 010 Editor | Edit binary mentah byte per byte |
| **Monitor** | Process Monitor, API Monitor | Pantau aktivitas file, registry, API calls |
| **PE Analysis** | PE-bear, CFF Explorer | Bongkar struktur file executable Windows |

---

## Roadmap Belajar

Ini peta jalan yang saya ikuti. Checklist ini akan terus di-update seiring progress.

### Fondasi
- [x] Setup repo & struktur folder
- [x] Dasar assembly x86 — register, instruksi, stack
- [x] Calling conventions, flags, addressing modes
- [ ] Struktur file PE (Portable Executable)
- [ ] Struktur file ELF (Executable and Linkable Format)
- [ ] Konsep OS: memory management, process, threads

### Tools & Praktek
- [x] Panduan Ghidra — navigasi, decompiler, XREF, patching
- [ ] Panduan x64dbg — breakpoint, stepping, memory view
- [ ] Panduan WinDbg — kernel debugging
- [ ] Menulis script otomasi (Python + pefile/capstone)

### Eksperimen
- [x] Password check crackme (source code)
- [ ] Solve crackme pertama (binary)
- [ ] Analisis malware sederhana (dalam VM)
- [ ] CTF challenge pertama

### Lanjutan
- [ ] Anti-debugging & anti-RE techniques
- [ ] Unpacking (UPX, custom packers)
- [ ] Shellcode analysis
- [ ] Windows internals (PEB, TEB, syscalls)

---

## Quick Start

Kalau kamu baru di RE dan menemukan repo ini, mulai dari sini:

**1. Baca teori dasar**
```
notes/assembly/01-dasar-assembly-x86.md        ← Mulai dari sini
notes/assembly/03-calling-conventions-flags-addressing.md
```

**2. Coba praktek**
```
notes/assembly/02-praktek-assembly.md           ← Compile & debug
scripts/coding/password_check.c                 ← Latihan pertama
```

**3. Belajar pakai Ghidra**
```
notes/assembly/04-re-dengan-ghidra.md           ← Bongkar binary pertamamu
```

> [!TIP]
> Jangan langsung loncat ke malware analysis atau CTF. Pastikan kamu paham assembly dasar dan bisa pakai debugger dulu. Fondasi yang kuat bikin semuanya lebih mudah.

---

## Tips untuk Pemula

> [!TIP]
> **Mulai dari string.** Saat buka binary di Ghidra/IDA, hal pertama yang harus dilakukan adalah cari string menarik (`"password"`, `"error"`, `"success"`). Dari situ, ikuti referensi (XREF) untuk menemukan fungsi penting.

> [!TIP]
> **Selalu pakai VM untuk analisis malware.** Jangan pernah jalankan binary mencurigakan di mesin utama. Gunakan VirtualBox/VMware dengan snapshot.

> [!TIP]
> **Baca decompiler dulu, assembly belakangan.** Output decompiler Ghidra sudah cukup untuk memahami logika program. Baru lihat assembly kalau butuh detail lebih.

> [!IMPORTANT]
> **Reverse engineering untuk belajar.** Repo ini dibuat untuk tujuan edukasi. Jangan gunakan teknik RE untuk hal-hal ilegal seperti membajak software atau menyebarkan malware.

---

## Resources yang Membantu

Kumpulan resource yang saya pakai selama belajar:

| Resource | Tipe | Link |
|:---------|:-----|:-----|
| crackmes.one | Latihan | https://crackmes.one |
| Malware Unicorn RE101 | Workshop | https://malwareunicorn.org/workshops/re101 |
| Begin.RE | Tutorial | https://www.begin.re |
| Ghidra Docs | Dokumentasi | https://ghidra-sre.org |
| x86 Instruction Reference | Referensi | https://www.felixcloutier.com/x86 |
| CTFtime | Kompetisi | https://ctftime.org |

---

## Progress Harian

Semua catatan perjalanan ada di folder **[diary/](diary/)**. Setiap tanggal push punya subfolder sendiri dengan catatan bernomor (`catatan-1.md`, `catatan-2.md`, dst).

```
diary/
└── 09-04-2026/
    └── catatan-1.md    ← Entry pertama: setup repo
```

---

<div align="center">

**Dibuat dengan rasa ingin tahu dan banyak kopi.**

Kalau repo ini membantu kamu belajar, kasih star ya.

</div>
