def trim_newline(s: str) -> str:
    return s.rstrip("\n")

def strip_leading_zeros(s: str) -> str:
    return s.lstrip("0") or "0"

def es_numero_no_negativo(s: str) -> bool:
    return s.isdigit()

def multiplicar_por_digito(a: str, d: int) -> str:
    if d == 0:
        return "0"
    carry = 0
    resultado = []
    for ch in reversed(a):
        prod = (ord(ch) - ord("0")) * d + carry
        resultado.append(chr((prod % 10) + ord("0")))
        carry = prod // 10
    if carry:
        resultado.append(chr(carry + ord("0")))
    return strip_leading_zeros("".join(reversed(resultado)))

def agregar_ceros_derecha(base: str, shift: int) -> str:
    return base + "0" * shift

def sumar_cadenas(x: str, y: str) -> str:
    ix, iy = len(x) - 1, len(y) - 1
    carry = 0
    resultado = []
    while ix >= 0 or iy >= 0 or carry:
        dx = int(x[ix]) if ix >= 0 else 0
        dy = int(y[iy]) if iy >= 0 else 0
        s = dx + dy + carry
        resultado.append(str(s % 10))
        carry = s // 10
        ix -= 1
        iy -= 1
    return strip_leading_zeros("".join(reversed(resultado)))

def print_der(etq: str, txt: str, w: int):
    print(f"{etq}{txt.rjust(w)}")

def linea(w: int):
    print("-" * (w + 3))

def main():
    print("Multiplicacion 'Algoritmo de la Primaria' (pasos resumidos)")
    a = input("Ingresa el primer numero: ")
    b = input("Ingresa el segundo numero: ")
    a = strip_leading_zeros(trim_newline(a))
    b = strip_leading_zeros(trim_newline(b))

    if not es_numero_no_negativo(a) or not es_numero_no_negativo(b):
        print("Error: solo numeros no negativos.")
        return

    # Caso de cero
    if a == "0" or b == "0":
        w = max(len(a), len(b), 1)
        print("\nPASOS POR DIGITO")
        print("(Alguno es 0, todas las filas parciales son 0)")
        print("\nDISPOSICION FINAL")
        print_der("   ", a, w)
        print_der(" x ", b, w)
        linea(w)
        print_der(" = ", "0", w)
        return

    filas = []
    print("\nPASOS POR DIGITO")
    for j, digito in enumerate(reversed(b)):
        dj = int(digito)
        base = multiplicar_por_digito(a, dj)
        fila = agregar_ceros_derecha(base, j)
        print(f"  {a} × {digito} = {base}")
        print(f"  → Fila (corrimiento {j}): {fila}")
        filas.append(fila)

    suma = "0"
    ancho_parcial = 1
    for fila in filas:
        suma = sumar_cadenas(suma, fila)
        if len(fila) > ancho_parcial:
            ancho_parcial = len(fila)

    w = max(len(a), len(b), len(suma), ancho_parcial)

    print("\nDISPOSICION FINAL")
    print_der("   ", a, w)
    print_der(" x ", b, w)
    linea(w)
    for fila in filas:
        print_der("   ", strip_leading_zeros(fila), w)
    linea(w)
    print_der(" = ", suma, w)

if __name__ == "__main__":
    main()
