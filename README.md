# CPU Süreç Zamanlayıcı Simülasyonu

Algoritma ve Programlama dersi grup ödevi. Program, verilen süreç listesini **FCFS**, **SJF** ve **Round Robin** algoritmalarıyla simüle eder ve sonuçları karşılaştırır. Gerçek süreç çalıştırmaz; zamanı bir sayaçla ilerletir.

## Grup Üyeleri

- Mert SEVİNÇ
- Çiğdem AYDIN

## Derleme ve Çalıştırma

```bash
gcc cpu_scheduling.c -o cpu_scheduling
./cpu_scheduling
```

Windows'ta ikinci komut: `.\cpu_scheduling.exe`

## Girdi

Program sırasıyla şunları ister:

1. Süreç sayısı (1-10)
2. Her süreç için geliş zamanı ve CPU süresi
3. Round Robin için quantum (en az 1)

## Algoritmalar

| Algoritma | Çalışma şekli | Eşitlik durumu |
|---|---|---|
| FCFS | İlk gelen süreç ilk çalışır, bitene kadar kesilmez | Küçük numara önce |
| SJF (kesintisiz) | Gelmiş süreçlerden CPU süresi en kısa olan seçilir | Erken gelen, sonra küçük numara |
| Round Robin | P1'den Pn'ye dolaşılır, her süreç en fazla quantum kadar çalışır | Sıra süreç numarasına göredir |

- Çalışabilecek süreç yoksa saat en erken geliş zamanına ilerler ve `BOS` bloğu eklenir.
- Bağlam değişimi, art arda gelen süreç blokları farklı süreçlere aitse sayılır. `BOS` blokları sayılmaz.

## Hesaplamalar

- Dönüş süresi = Bitiş zamanı - Geliş zamanı
- Bekleme süresi = Dönüş süresi - CPU süresi

## Fonksiyonlar

| Fonksiyon | Görevi |
|---|---|
| `sifirla` | Gantt sayacını, bağlam değişimi sayacını ve son süreci sıfırlar |
| `gantt_ekle` | Gantt şemasına blok ekler ve bağlam değişimini sayar |
| `gantt_yazdir` | Gantt şemasını satırda en fazla 6 blok olacak şekilde yazar |
| `ortalamalar` | Ortalama bekleme ve dönüş süresini pointer parametrelerle döndürür |
| `rapor` | Gantt şemasını, sonuç tablosunu ve özeti yazar |
| `fcfs` | FCFS simülasyonunu yapar |
| `sjf` | SJF simülasyonunu yapar |
| `round_robin` | Round Robin simülasyonunu yapar |
| `karsilastir` | Üç algoritmanın ortalama beklemesini karşılaştırır |
| `main` | Girdiyi okur ve diğer fonksiyonları çağırır |

## Veri Yapısı

Süreç bilgileri paralel dizilerde tutulur: `gelis[i]`, `cpu[i]` ve `bitis[i]` aynı sürece (P(i+1)) aittir.

Global değişkenler:

| Değişken | Anlamı |
|---|---|
| `ganttNo` | Bloğun süreç numarası (boşta için 0) |
| `ganttBas` | Bloğun başlangıç zamanı |
| `ganttBit` | Bloğun bitiş zamanı |
| `ganttSayac` | Blok sayısı |
| `baglam` | Bağlam değişimi sayısı |
| `sonSurec` | Son çalışan süreç |

## Örnek

Girdi: 3 süreç, quantum = 2

| Süreç | Geliş | CPU süresi |
|---|---|---|
| P1 | 0 | 5 |
| P2 | 1 | 3 |
| P3 | 2 | 1 |

Gantt şemaları:

```
FCFS:        [P1 0-5] [P2 5-8] [P3 8-9]
SJF:         [P1 0-5] [P3 5-6] [P2 6-9]
Round Robin: [P1 0-2] [P2 2-4] [P3 4-5] [P1 5-7] [P2 7-8] [P1 8-9]
```

| Algoritma | Ort. bekleme | Ort. dönüş | Bağlam değişimi |
|---|---|---|---|
| FCFS | 3.33 | 6.33 | 2 |
| SJF | 2.67 | 5.67 | 2 |
| Round Robin | 3.33 | 6.33 | 5 |

En düşük ortalama bekleme: **SJF (2.67)**

## Kısıtlar

- Yalnızca `stdio.h` kullanılmıştır; `struct` ve `malloc/free` yoktur.
- En fazla 10 süreç girilebilir.
- Gantt şeması en fazla 500 blok tutar. Bu sınır aşılırsa şema eksik, bağlam değişimi sayısı yanlış olur.
- Geliş zamanı 0 veya daha büyük, CPU süresi en az 1 tam sayı olmalıdır. Program bu iki değeri kontrol etmez.
