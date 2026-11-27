#include <stdio.h>

/* Grup uyeleri: Cigdem AYDIN ve Mert SEVINC */

#define MAKS_SUREC 10
#define MAKS_GANTT 500

/* Fonksiyonlar grup uyeleri tarafindan ortak hazirlanmistir. */
int ganttNo[MAKS_GANTT];
int ganttBas[MAKS_GANTT];
int ganttBit[MAKS_GANTT];
int ganttSayac = 0;
int baglam = 0;
int sonSurec = 0;

/* Yazan: Cigdem AYDIN ve Mert SEVINC (ortak calisma) */
/*
   Her algoritmadan once Gantt sayaclarini ve baglam degisimini sifirlar. */
void sifirla(void) {
    ganttSayac = 0;
    baglam = 0;
    sonSurec = 0;
}

/* Yazan: Cigdem AYDIN ve Mert SEVINC (ortak calisma) */
/*
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

    if (no != 0) {
        if (sonSurec != 0 && sonSurec != no) {
            baglam++;
        }
        sonSurec = no;
    }
}

/* Yazan: Cigdem AYDIN ve Mert SEVINC (ortak calisma) */
/*
   Kaydedilmis Gantt bloklarini yazdirir, satirda en fazla 6 blok. */
void gantt_yazdir(void) {
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

/* Yazan: Cigdem AYDIN ve Mert SEVINC (ortak calisma) */
/*
   Ortalama bekleme ve ortalama donus suresini pointer parametrelerle verir. */
void ortalamalar(int bekleme[], int donus[], int n, float *ortB, float *ortD) {
    int i;
    int topB = 0;
    int topD = 0;

    for (i = 0; i < n; i++) {
        topB += bekleme[i];
        topD += donus[i];
    }

    *ortB = (float)topB / n;
    *ortD = (float)topD / n;
}

/* Yazan: Cigdem AYDIN ve Mert SEVINC (ortak calisma) */
/*
   Bir algoritmanin Gantt, tablo, ortalamalar ve baglam bilgilerini yazdirir. */
void rapor(char ad[], int n, int gelis[], int cpu[], int bitis[], float *ortB) {
    int donus[MAKS_SUREC];
    int bekleme[MAKS_SUREC];
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

/* Yazan: Cigdem AYDIN ve Mert SEVINC (ortak calisma) */
/*
   FCFS: Gelis zamanina gore siralar, her sureci bitene kadar calistirir. */
void fcfs(int n, int gelis[], int cpu[], int bitis[]) {
    int sira[MAKS_SUREC];
    int i, j, k, gecici;
    int zaman = 0;

    sifirla();

    for (i = 0; i < n; i++) {
        sira[i] = i;
    }

    /* Baloncuk siralama: esit gelis zamaninda kucuk numara onde kalir. */
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

        if (zaman < gelis[k]) {
            gantt_ekle(0, zaman, gelis[k]);
            zaman = gelis[k];
        }

        gantt_ekle(k + 1, zaman, zaman + cpu[k]);
        zaman += cpu[k];
        bitis[k] = zaman;
    }
}

/* Yazan: Cigdem AYDIN ve Mert SEVINC (ortak calisma) */
/*
   SJF (kesintisiz): Gelmis ve bitmemis sureclerden CPU suresi en kisa olani secer.
   Esitlikte once erken gelen, sonra kucuk numarali surec secilir. */
void sjf(int n, int gelis[], int cpu[], int bitis[]) {
    int bitti[MAKS_SUREC];
    int i, secilen, enErken;
    int zaman = 0;
    int biten = 0;

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

        if (secilen == -1) {
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
            zaman += cpu[secilen];
            bitis[secilen] = zaman;
            bitti[secilen] = 1;
            biten++;
        }
    }
}

/* Yazan: Cigdem AYDIN ve Mert SEVINC (ortak calisma) */
/*
   Round Robin: P1'den Pn'ye dolasir; gelen surec en fazla quantum kadar calisir. */
void round_robin(int n, int gelis[], int cpu[], int bitis[], int quantum) {
    int kalan[MAKS_SUREC];
    int i, sure, enErken, calisti;
    int zaman = 0;
    int biten = 0;

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
                zaman += sure;
                kalan[i] -= sure;
                calisti = 1;

                if (kalan[i] == 0) {
                    bitis[i] = zaman;
                    biten++;
                }
            }
        }

        if (calisti == 0) {
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

/* Yazan: Cigdem AYDIN ve Mert SEVINC (ortak calisma) */
/*
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
        ad[0] = 'S';
        ad[1] = 'J';
        ad[2] = 'F';
        ad[3] = '\0';
    }

    if (bRr < enIyi) {
        enIyi = bRr;
        ad[0] = 'R';
        ad[1] = 'R';
        ad[2] = '\0';
    }

    printf("En dusuk ortalama bekleme: %s (%.2f)\n", ad, enIyi);
}

/* Yazan: Cigdem AYDIN ve Mert SEVINC (ortak calisma) */
/*
   Verileri okur, uc algoritmayi calistirir ve sonuclari karsilastirir. */
int main(void) {
    int gelis[MAKS_SUREC];
    int cpu[MAKS_SUREC];
    int bitis[MAKS_SUREC];
    int n, i, quantum;
    float bFcfs, bSjf, bRr;

    printf("Kac surec var (1-%d)? ", MAKS_SUREC);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAKS_SUREC) {
        printf("Gecersiz surec sayisi.\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        printf("P%d gelis ve CPU suresi: ", i + 1);

        if (scanf("%d %d", &gelis[i], &cpu[i]) != 2 ||
            gelis[i] < 0 || cpu[i] < 1) {
            printf("Gecersiz giris! Gelis zamani 0 veya daha buyuk, CPU suresi en az 1 olmali.\n");
            return 1;
        }
    }

    printf("Round Robin icin quantum: ");
    if (scanf("%d", &quantum) != 1 || quantum < 1) {
        printf("Gecersiz quantum. Quantum en az 1 olmali.\n");
        return 1;
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
