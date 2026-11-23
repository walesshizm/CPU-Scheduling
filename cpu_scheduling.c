#include <stdio.h>

#define MAKS_SUREC 10
#define MAKS_GANTT 500

/* ---------- GLOBAL DEGISKENLER ----------
   (Odevdeki izinli global listesiyle karsilastirin!) */
int ganttNo[MAKS_GANTT];   /* blogun surec numarasi (BOS icin 0) */
int ganttBas[MAKS_GANTT];  /* blogun baslangic zamani */
int ganttBit[MAKS_GANTT];  /* blogun bitis zamani */
int ganttSayac = 0;        /* kac blok var */
int baglam = 0;            /* baglam degisimi sayisi */
int sonSurec = 0;          /* en son calisan surec (0 = henuz yok) */

/* Yazan: (isim yazin)
   Her algoritmadan once Gantt sayaclarini ve baglam degisimini sifirlar. */
void sifirla() {
    ganttSayac = 0;
    baglam = 0;
    sonSurec = 0;
}

/* Yazan: (isim yazin)
   Gantt'a bir blok ekler. no=0 ise BOS blogudur.
   Bir surec blogu, bir onceki SUREC blogundan farkliysa baglam degisimi +1. */
void gantt_ekle(int no, int bas, int bit) {
    if (ganttSayac >= MAKS_GANTT) {
        return;
    }
    ganttNo[ganttSayac] = no;
    ganttBas[ganttSayac] = bas;
    ganttBit[ganttSayac] = bit;
    ganttSayac++;
    if (no != 0) {                         /* BOS sayilmaz */
        if (sonSurec != 0 && sonSurec != no) {
            baglam++;
        }
        sonSurec = no;
    }
}

/* Yazan: (isim yazin)
   Kaydedilmis Gantt bloklarini yazdirir, satirda en fazla 6 blok. */
void gantt_yazdir() {
    int i;
    for (i = 0; i < ganttSayac; i++) {
        if (ganttNo[i] == 0) {
            printf("[BOS %d-%d] ", ganttBas[i], ganttBit[i]);
        } else {
            printf("[P%d %d-%d] ", ganttNo[i], ganttBas[i], ganttBit[i]);
        }
        if ((i + 1) % 6 == 0) {
            printf("\n");
        }
    }
    if (ganttSayac % 6 != 0) {
        printf("\n");
    }
}

/* Yazan: (isim yazin)
   Ortalama bekleme ve ortalama donus suresini hesaplar.
   Iki sonuc dondurmesi gerektigi icin sonuclari pointer parametrelerle verir. */
void ortalamalar(int bekleme[], int donus[], int n, float *ortB, float *ortD) {
    int i, topB = 0, topD = 0;
    for (i = 0; i < n; i++) {
        topB = topB + bekleme[i];
        topD = topD + donus[i];
    }
    *ortB = (float)topB / n;
    *ortD = (float)topD / n;
}

/* Yazan: (isim yazin)
   Bir algoritmanin sonucunu yazdirir: Gantt, tablo, ortalamalar, baglam.
   Ortalama beklemeyi *ortB ile geri verir (karsilastirma icin). */
void rapor(char ad[], int n, int gelis[], int cpu[], int bitis[], float *ortB) {
    int donus[MAKS_SUREC], bekleme[MAKS_SUREC];
    int i;
    float ortD;

    for (i = 0; i < n; i++) {
        donus[i] = bitis[i] - gelis[i];
        bekleme[i] = donus[i] - cpu[i];
    }
    ortalamalar(bekleme, donus, n, ortB, &ortD);

    printf("\n===== %s =====\n", ad);
    gantt_yazdir();
    printf("\nNo   Gelis  CPU  Bitis  Donus  Bekleme\n");
    for (i = 0; i < n; i++) {
        printf("P%-3d %-6d %-4d %-6d %-6d %-6d\n", i + 1, gelis[i], cpu[i],
               bitis[i], donus[i], bekleme[i]);
    }
    printf("Ortalama bekleme: %.2f\n", *ortB);
    printf("Ortalama donus  : %.2f\n", ortD);
    printf("Baglam degisimi : %d\n", baglam);
}

/* Yazan: (isim yazin)
   FCFS: gelis zamanina gore siralar, her sureci bitene kadar calistirir. */
void fcfs(int n, int gelis[], int cpu[], int bitis[]) {
    int sira[MAKS_SUREC];
    int i, j, k, gecici, zaman = 0;

    sifirla();
    for (i = 0; i < n; i++) {
        sira[i] = i;
    }
    /* baloncuk siralama: esit gelisde kucuk numara onde kalir */
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (gelis[sira[j]] > gelis[sira[j + 1]]) {
                gecici = sira[j];
                sira[j] = sira[j + 1];
                sira[j + 1] = gecici;
            }
        }
    }
    for (i = 0; i < n; i++) {
        k = sira[i];
        if (zaman < gelis[k]) {            /* surec henuz gelmedi: CPU bos */
            gantt_ekle(0, zaman, gelis[k]);
            zaman = gelis[k];
        }
        gantt_ekle(k + 1, zaman, zaman + cpu[k]);
        zaman = zaman + cpu[k];
        bitis[k] = zaman;
    }
}

/* Yazan: (isim yazin)
   SJF (kesintisiz): CPU bosalinca gelmis ve bitmemis surecler arasindan
   CPU suresi en kisa olani secer. Esitlikte once erken gelen, sonra kucuk numara. */
void sjf(int n, int gelis[], int cpu[], int bitis[]) {
    int bitti[MAKS_SUREC];
    int i, secilen, enErken, zaman = 0, biten = 0;

    sifirla();
    for (i = 0; i < n; i++) {
        bitti[i] = 0;
    }
    while (biten < n) {
        secilen = -1;
        for (i = 0; i < n; i++) {
            if (bitti[i] == 0 && gelis[i] <= zaman) {
                if (secilen == -1 || cpu[i] < cpu[secilen] ||
                    (cpu[i] == cpu[secilen] && gelis[i] < gelis[secilen])) {
                    secilen = i;
                }
            }
        }
        if (secilen == -1) {               /* gelmis surec yok: CPU bos */
            enErken = -1;
            for (i = 0; i < n; i++) {
                if (bitti[i] == 0) {
                    if (enErken == -1 || gelis[i] < gelis[enErken]) {
                        enErken = i;
                    }
                }
            }
            gantt_ekle(0, zaman, gelis[enErken]);
            zaman = gelis[enErken];
        } else {
            gantt_ekle(secilen + 1, zaman, zaman + cpu[secilen]);
            zaman = zaman + cpu[secilen];
            bitis[secilen] = zaman;
            bitti[secilen] = 1;
            biten++;
        }
    }
}

/* Yazan: (isim yazin)
   Round Robin: P1'den Pn'ye dolasir. Gelmis ve bitmemis surec en fazla
   quantum kadar calisir. Bir turda kimse calismazsa saat en erken gelise atlar. */
void round_robin(int n, int gelis[], int cpu[], int bitis[], int quantum) {
    int kalan[MAKS_SUREC];
    int i, sure, enErken, calisti, zaman = 0, biten = 0;

    sifirla();
    for (i = 0; i < n; i++) {
        kalan[i] = cpu[i];
    }
    while (biten < n) {
        calisti = 0;
        for (i = 0; i < n; i++) {
            if (kalan[i] > 0 && gelis[i] <= zaman) {
                sure = quantum;
                if (kalan[i] < quantum) {
                    sure = kalan[i];
                }
                gantt_ekle(i + 1, zaman, zaman + sure);
                zaman = zaman + sure;
                kalan[i] = kalan[i] - sure;
                calisti = 1;
                if (kalan[i] == 0) {
                    bitis[i] = zaman;
                    biten++;
                }
            }
        }
        if (calisti == 0) {                /* tur boyunca kimse calismadi: CPU bos */
            enErken = -1;
            for (i = 0; i < n; i++) {
                if (kalan[i] > 0) {
                    if (enErken == -1 || gelis[i] < gelis[enErken]) {
                        enErken = i;
                    }
                }
            }
            gantt_ekle(0, zaman, gelis[enErken]);
            zaman = gelis[enErken];
        }
    }
}

/* Yazan: (isim yazin)
   Uc algoritmanin ortalama beklemesini karsilastirir.
   Esitlikte FCFS, SJF, RR sirasi gecerlidir. */
void karsilastir(float bFcfs, float bSjf, float bRr) {
    float enIyi = bFcfs;
    char ad[20] = "FCFS";

    printf("\n===== KARSILASTIRMA =====\n");
    printf("FCFS ortalama bekleme: %.2f\n", bFcfs);
    printf("SJF  ortalama bekleme: %.2f\n", bSjf);
    printf("RR   ortalama bekleme: %.2f\n", bRr);

    if (bSjf < enIyi) {
        enIyi = bSjf;
        ad[0] = 'S'; ad[1] = 'J'; ad[2] = 'F'; ad[3] = '\0';
    }
    if (bRr < enIyi) {
        enIyi = bRr;
        ad[0] = 'R'; ad[1] = 'R'; ad[2] = '\0';
    }
    printf("En dusuk ortalama bekleme: %s (%.2f)\n", ad, enIyi);
}

/* Yazan: (isim yazin)
   Verileri okur, uc algoritmayi calistirir, sonuclari karsilastirir. */
int main() {
    int gelis[MAKS_SUREC], cpu[MAKS_SUREC], bitis[MAKS_SUREC];
    int n, i, quantum;
    float bFcfs, bSjf, bRr;

    printf("Kac surec var (1-%d)? ", MAKS_SUREC);
    scanf("%d", &n);
    if (n < 1 || n > MAKS_SUREC) {
        printf("Gecersiz surec sayisi.\n");
        return 0;
    }
    for (i = 0; i < n; i++) {
        printf("P%d gelis ve cpu suresi: ", i + 1);
        scanf("%d %d", &gelis[i], &cpu[i]);
    }
    printf("Round Robin icin quantum: ");
    scanf("%d", &quantum);
    if (quantum < 1) {
        printf("Quantum en az 1 olmali.\n");
        return 0;
    }

    fcfs(n, gelis, cpu, bitis);
    rapor("FCFS", n, gelis, cpu, bitis, &bFcfs);

    sjf(n, gelis, cpu, bitis);
    rapor("SJF", n, gelis, cpu, bitis, &bSjf);

    round_robin(n, gelis, cpu, bitis, quantum);
    rapor("ROUND ROBIN", n, gelis, cpu, bitis, &bRr);

    karsilastir(bFcfs, bSjf, bRr);
    return 0;
}