#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Utilidades */

static void trim_newline(char *s){
    size_t n=strlen(s);
    if(n && s[n-1]=='\n') s[n-1]='\0';
}

static void strip_leading_zeros_inplace(char *s){
    size_t n=strlen(s), i=0;
    while(i+1<n && s[i]=='0') i++;
    if(i>0) memmove(s, s+i, n-i+1);
}

static int es_numero_no_negativo(const char *s){
    if(!*s) return 0;
    for(const char *p=s; *p; ++p) if(!isdigit((unsigned char)*p)) return 0;
    return 1;
}

/* Multiplica cadena decimal a por un dígito d (0..9). Devuelve cadena nueva (sin ceros a la izq). */
static char* multiplicar_por_digito(const char *a, int d){
    if(d==0) { 
        char *z=malloc(2); 
        z[0]='0'; 
        z[1]='\0'; 
        return z; 
    }
    int n=(int)strlen(a);
    char *rev=malloc(n+3); // temporal en reversa
    int k=0, carry=0;
    for(int i=n-1;i>=0;--i){
        int prod=(a[i]-'0')*d + carry;
        rev[k++]=(char)('0'+(prod%10));
        carry=prod/10;
    }
    if(carry) rev[k++]=(char)('0'+carry);
    // invertir
    char *out=malloc(k+1);
    for(int i=0;i<k;i++) out[i]=rev[k-1-i];
    out[k]='\0';
    free(rev);
    strip_leading_zeros_inplace(out);
    return out;
}

/* Concatena 'shift' ceros al final de base. */
static char* agregar_ceros_derecha(const char *base, int shift){
    int n=(int)strlen(base);
    char *out=malloc(n+shift+1);
    memcpy(out, base, n);
    for(int i=0;i<shift;i++) out[n+i]='0';
    out[n+shift]='\0';
    return out;
}

/* Suma dos enteros no negativos en decimal (cadenas). */
static char* sumar_cadenas(const char *x, const char *y){
    int nx=(int)strlen(x), ny=(int)strlen(y);
    int n = (nx>ny?nx:ny) + 1;
    char *rev=malloc(n+1);
    int ix=nx-1, iy=ny-1, k=0, carry=0;
    while(ix>=0 || iy>=0 || carry){
        int dx = (ix>=0? x[ix]-'0' : 0);
        int dy = (iy>=0? y[iy]-'0' : 0);
        int s = dx + dy + carry;
        rev[k++]=(char)('0'+(s%10));
        carry=s/10;
        ix--; iy--;
    }
    // invertir
    char *out=malloc(k+1);
    for(int i=0;i<k;i++) out[i]=rev[k-1-i];
    out[k]='\0';
    free(rev);
    strip_leading_zeros_inplace(out);
    return out;
}

/* Imprime texto alineado a la derecha en ancho 'w' con etiqueta opcional */
static void print_der(const char *etq, const char *txt, int w){
    int len=(int)strlen(txt);
    int sp = (w>len? w-len : 0);
    if(etq && *etq) printf("%s", etq);
    for(int i=0;i<sp;i++) putchar(' ');
    printf("%s\n", txt);
}

static void linea(int w){
    for(int i=0;i<w+3;i++) putchar('-');
    putchar('\n');
}

int main(void){
    char a[4096], b[4096];
    printf("Multiplicacion 'Algoritmo de la Primaria' (pasos resumidos)\n");
    printf("Ingresa el primer numero: ");
    if(!fgets(a, sizeof(a), stdin)) return 1;
    trim_newline(a);
    printf("Ingresa el segundo numero: ");
    if(!fgets(b, sizeof(b), stdin)) return 1;
    trim_newline(b);

    if(!es_numero_no_negativo(a) || !es_numero_no_negativo(b)){
        fprintf(stderr,"Error: solo numeros no negativos.\n");
        return 1;
    }
    strip_leading_zeros_inplace(a);
    strip_leading_zeros_inplace(b);

    // Si alguno es 0
    if((a[0]=='0' && a[1]=='\0') || (b[0]=='0' && b[1]=='\0')){
        int w = (int)strlen(a) + (int)strlen(b);
        if(w<1) w=1;
        printf("\nPASOS POR DIGITO\n");
        printf("(Alguno es 0, todas las filas parciales son 0)\n");
        printf("\nDISPOSICION FINAL\n");
        print_der("   ", a, w);
        print_der(" x ", b, w);
        linea(w);
        print_der(" = ", "0", w);
        return 0;
    }

    int L2=(int)strlen(b);
    // Guardamos filas parciales ya con corrimiento
    char **filas = malloc(sizeof(char*) * L2);
    if(!filas){ 
        fprintf(stderr,"Memoria insuficiente.\n"); 
        return 1; 
    }

    printf("\nPASOS POR DIGITO\n");
    for(int j=L2-1, shift=0; j>=0; --j, ++shift){
        int dj = b[j]-'0';
        char *base = multiplicar_por_digito(a, dj);          // a × dígito
        char *fila  = agregar_ceros_derecha(base, shift);    // corrimiento (ceros)
        printf("  %s × %c = %s\n", a, b[j], base);
        printf("  → Fila (corrimiento %d): %s\n", shift, fila);
        filas[(L2-1)-j] = fila; // guardamos en orden de arriba hacia abajo
        free(base);
    }

    // Suma de todas las filas
    char *suma = malloc(2); 
    suma[0]='0'; 
    suma[1]='\0';
    int ancho_parcial = (int)strlen(suma);
    for(int r=0; r<L2; ++r){
        char *tmp = sumar_cadenas(suma, filas[r]);
        free(suma);
        suma = tmp;
        if((int)strlen(filas[r]) > ancho_parcial) ancho_parcial = (int)strlen(filas[r]);
    }

    // Ancho para mostrar todo bien alineado
    int w = (int)strlen(suma);
    if((int)strlen(a) > w) w = (int)strlen(a);
    if((int)strlen(b) > w) w = (int)strlen(b);
    if(ancho_parcial > w) w = ancho_parcial;

    // Disposición final como en papel
    printf("\nDISPOSICION FINAL\n");
    print_der("   ", a, w);
    print_der(" x ", b, w);
    linea(w);
    for(int r=0; r<L2; ++r){
        char *cop = strdup(filas[r]);
        strip_leading_zeros_inplace(cop);
        print_der("   ", (*cop?cop:"0"), w);
        free(cop);
    }
    linea(w);
    print_der(" = ", suma, w);

    // liberar memoria
    for(int r=0;r<L2;++r) free(filas[r]);
    free(filas);
    free(suma);
    return 0;
}
