#!/bin/bash

# ==============================================================================
#  rush-02 — Test Suite Completo
# ==============================================================================

BINARY="./rush-02"
PASS=0
FAIL=0
SKIP=0

GREEN='\033[0;32m'
RED='\033[0;31m'
CYAN='\033[0;36m'
GRAY='\033[0;90m'
RESET='\033[0m'

# ------------------------------------------------------------------------------
#  Helpers
# ------------------------------------------------------------------------------

run_test()
{
    local desc="$1"
    local expected="$2"
    shift 2
    local actual
    actual=$("$@" 2>/dev/null)
    if [ "$actual" = "$expected" ]; then
        echo -e "  ${GREEN}PASS${RESET}  $desc"
        PASS=$((PASS + 1))
    else
        echo -e "  ${RED}FAIL${RESET}  $desc"
        echo -e "        ${GRAY}esperado : $(echo "$expected" | cat -A)${RESET}"
        echo -e "        ${GRAY}obtido   : $(echo "$actual"   | cat -A)${RESET}"
        FAIL=$((FAIL + 1))
    fi
}

section()
{
    echo ""
    echo -e "${CYAN}── $1${RESET}"
}

make_dict()
{
    cat > "$1" << 'DICT'
0: zero
1: one
2: two
3: three
4: four
5: five
6: six
7: seven
8: eight
9: nine
10: ten
11: eleven
12: twelve
13: thirteen
14: fourteen
15: fifteen
16: sixteen
17: seventeen
18: eighteen
19: nineteen
20: twenty
30: thirty
40: forty
50: fifty
60: sixty
70: seventy
80: eighty
90: ninety
100: hundred
1000: thousand
1000000: million
1000000000: billion
1000000000000: trillion
1000000000000000: quadrillion
1000000000000000000: quintillion
1000000000000000000000: sextillion
1000000000000000000000000: septillion
1000000000000000000000000000: octillion
1000000000000000000000000000000: nonillion
1000000000000000000000000000000000: decillion
1000000000000000000000000000000000000: undecillion
DICT
    shift
    for extra in "$@"; do echo "$extra" >> "$1"; done
}

if [ ! -f "$BINARY" ]; then
    echo -e "${RED}Binário '$BINARY' não encontrado. Rode 'make' primeiro.${RESET}"
    exit 1
fi

# ==============================================================================
section "ZERO E VARIAÇÕES"
# ==============================================================================

run_test "0"       "zero" $BINARY 0
run_test "00"      "zero" $BINARY 00
run_test "000"     "zero" $BINARY 000
run_test "0000"    "zero" $BINARY 0000
run_test "00000"   "zero" $BINARY 00000

# ==============================================================================
section "DÍGITOS ÚNICOS (1–9)"
# ==============================================================================

run_test "1" "one"   $BINARY 1
run_test "2" "two"   $BINARY 2
run_test "3" "three" $BINARY 3
run_test "4" "four"  $BINARY 4
run_test "5" "five"  $BINARY 5
run_test "6" "six"   $BINARY 6
run_test "7" "seven" $BINARY 7
run_test "8" "eight" $BINARY 8
run_test "9" "nine"  $BINARY 9

# ==============================================================================
section "DEZ A DEZENOVE (lookup direto)"
# ==============================================================================

run_test "10" "ten"       $BINARY 10
run_test "11" "eleven"    $BINARY 11
run_test "12" "twelve"    $BINARY 12
run_test "13" "thirteen"  $BINARY 13
run_test "14" "fourteen"  $BINARY 14
run_test "15" "fifteen"   $BINARY 15
run_test "16" "sixteen"   $BINARY 16
run_test "17" "seventeen" $BINARY 17
run_test "18" "eighteen"  $BINARY 18
run_test "19" "nineteen"  $BINARY 19

# ==============================================================================
section "MÚLTIPLOS DE 10 (20–90)"
# ==============================================================================

run_test "20" "twenty"  $BINARY 20
run_test "30" "thirty"  $BINARY 30
run_test "40" "forty"   $BINARY 40
run_test "50" "fifty"   $BINARY 50
run_test "60" "sixty"   $BINARY 60
run_test "70" "seventy" $BINARY 70
run_test "80" "eighty"  $BINARY 80
run_test "90" "ninety"  $BINARY 90

# ==============================================================================
section "DEZENAS COM UNIDADES"
# ==============================================================================

run_test "21" "twenty one"   $BINARY 21
run_test "22" "twenty two"   $BINARY 22
run_test "29" "twenty nine"  $BINARY 29
run_test "31" "thirty one"   $BINARY 31
run_test "42" "forty two"    $BINARY 42
run_test "55" "fifty five"   $BINARY 55
run_test "66" "sixty six"    $BINARY 66
run_test "71" "seventy one"  $BINARY 71
run_test "88" "eighty eight" $BINARY 88
run_test "99" "ninety nine"  $BINARY 99

# ==============================================================================
section "MÚLTIPLOS DE 100"
# ==============================================================================

run_test "100" "one hundred"   $BINARY 100
run_test "200" "two hundred"   $BINARY 200
run_test "300" "three hundred" $BINARY 300
run_test "500" "five hundred"  $BINARY 500
run_test "900" "nine hundred"  $BINARY 900

# ==============================================================================
section "CENTENAS COMPOSTAS"
# ==============================================================================

run_test "101" "one hundred one"              $BINARY 101
run_test "110" "one hundred ten"              $BINARY 110
run_test "111" "one hundred eleven"           $BINARY 111
run_test "115" "one hundred fifteen"          $BINARY 115
run_test "119" "one hundred nineteen"         $BINARY 119
run_test "120" "one hundred twenty"           $BINARY 120
run_test "121" "one hundred twenty one"       $BINARY 121
run_test "199" "one hundred ninety nine"      $BINARY 199
run_test "350" "three hundred fifty"          $BINARY 350
run_test "512" "five hundred twelve"          $BINARY 512
run_test "999" "nine hundred ninety nine"     $BINARY 999

# ==============================================================================
section "MILHARES EXATOS"
# ==============================================================================

run_test "1000"   "one thousand"          $BINARY 1000
run_test "2000"   "two thousand"          $BINARY 2000
run_test "10000"  "ten thousand"          $BINARY 10000
run_test "11000"  "eleven thousand"       $BINARY 11000
run_test "20000"  "twenty thousand"       $BINARY 20000
run_test "100000" "one hundred thousand"  $BINARY 100000
run_test "900000" "nine hundred thousand" $BINARY 900000

# ==============================================================================
section "MILHARES COMPOSTOS"
# ==============================================================================

run_test "1001"   "one thousand one"                          $BINARY 1001
run_test "1010"   "one thousand ten"                          $BINARY 1010
run_test "1011"   "one thousand eleven"                       $BINARY 1011
run_test "1100"   "one thousand one hundred"                  $BINARY 1100
run_test "1111"   "one thousand one hundred eleven"           $BINARY 1111
run_test "1999"   "one thousand nine hundred ninety nine"     $BINARY 1999
run_test "10001"  "ten thousand one"                          $BINARY 10001
run_test "10100"  "ten thousand one hundred"                  $BINARY 10100
run_test "42000"  "forty two thousand"                        $BINARY 42000
run_test "42042"  "forty two thousand forty two"              $BINARY 42042
run_test "100001" "one hundred thousand one"                  $BINARY 100001
run_test "999999" "nine hundred ninety nine thousand nine hundred ninety nine" \
                                                              $BINARY 999999

# ==============================================================================
section "ZEROS NO MEIO (grupos vazios)"
# ==============================================================================

run_test "1000001"         "one million one"               $BINARY 1000001
run_test "1000010"         "one million ten"               $BINARY 1000010
run_test "1000100"         "one million one hundred"       $BINARY 1000100
run_test "1001000"         "one million one thousand"      $BINARY 1001000
run_test "1000000001"      "one billion one"               $BINARY 1000000001
run_test "1001001001"      "one billion one million one thousand one" \
                                                           $BINARY 1001001001
run_test "1000000000000001" "one quadrillion one"           $BINARY 1000000000000001

# ==============================================================================
section "NÚMEROS GRANDES"
# ==============================================================================

run_test "1000000"          "one million"       $BINARY 1000000
run_test "2000000"          "two million"       $BINARY 2000000
run_test "1000000000"       "one billion"       $BINARY 1000000000
run_test "1000000000000"    "one trillion"      $BINARY 1000000000000
run_test "1000000000000000" "one quadrillion"   $BINARY 1000000000000000

# ==============================================================================
section "ZEROS À ESQUERDA"
# ==============================================================================

run_test "01"    "one"          $BINARY 01
run_test "001"   "one"          $BINARY 001
run_test "007"   "seven"        $BINARY 007
run_test "010"   "ten"          $BINARY 010
run_test "011"   "eleven"       $BINARY 011
run_test "042"   "forty two"    $BINARY 042
run_test "0100"  "one hundred"  $BINARY 0100
run_test "00042" "forty two"    $BINARY 00042

# ==============================================================================
section "EXEMPLOS DO SUBJECT"
# ==============================================================================

run_test "subject: 42"      "forty two"            $BINARY 42
run_test "subject: 0"       "zero"                 $BINARY 0
run_test "subject: 10.4"    "Error"                $BINARY 10.4
run_test "subject: 100000"  "one hundred thousand" $BINARY 100000

# ==============================================================================
section "INPUTS INVÁLIDOS → Error"
# ==============================================================================

run_test "float com ponto"        "Error" $BINARY 10.4
run_test "float com vírgula"      "Error" $BINARY 10,4
run_test "negativo"               "Error" $BINARY -1
run_test "negativo grande"        "Error" $BINARY -1000
run_test "só letras"              "Error" $BINARY abc
run_test "letras e números"       "Error" $BINARY 12ab
run_test "número e letras"        "Error" $BINARY 42xyz
run_test "espaço no número"       "Error" $BINARY "1 2"
run_test "string vazia"           "Error" $BINARY ""
run_test "sem argumentos"         "Error" $BINARY
run_test "três argumentos"        "Error" $BINARY 1 2 3
run_test "quatro argumentos"      "Error" $BINARY 1 2 3 4
run_test "sinal de mais"          "Error" $BINARY +42
run_test "underline"              "Error" $BINARY 1_000
run_test "hex com 0x"             "Error" $BINARY 0x1F
run_test "letras maiúsculas"      "Error" $BINARY ABC
run_test "número com ponto final" "Error" $BINARY "42."
run_test "número com barra"       "Error" $BINARY "1/2"
run_test "40 dígitos"             "Error" \
    $BINARY 9999999999999999999999999999999999999999

# ==============================================================================
section "2 ARGUMENTOS: dicionário customizado"
# ==============================================================================

D=$(mktemp /tmp/rush02_XXXX.dict)
make_dict "$D"

run_test "2 args: 0"       "zero"             $BINARY "$D" 0
run_test "2 args: 42"      "forty two"        $BINARY "$D" 42
run_test "2 args: 100"     "one hundred"      $BINARY "$D" 100
run_test "2 args: 1000"    "one thousand"     $BINARY "$D" 1000
run_test "2 args: número inválido → Error" "Error" $BINARY "$D" abc
rm -f "$D"

# ==============================================================================
section "DICIONÁRIO CUSTOMIZADO EM PORTUGUÊS"
# ==============================================================================

PT=$(mktemp /tmp/rush02_XXXX.dict)
cat > "$PT" << 'DICT'
0: zero
1: um
2: dois
3: tres
4: quatro
5: cinco
6: seis
7: sete
8: oito
9: nove
10: dez
11: onze
12: doze
13: treze
14: catorze
15: quinze
16: dezesseis
17: dezessete
18: dezoito
19: dezenove
20: vinte
30: trinta
40: quarenta
50: cinquenta
60: sessenta
70: setenta
80: oitenta
90: noventa
100: cem
1000: mil
1000000: milhao
1000000000: bilhao
1000000000000: trilhao
1000000000000000: quatrilhao
1000000000000000000: quintilhao
1000000000000000000000: sextilhao
1000000000000000000000000: septilhao
1000000000000000000000000000: octilhao
1000000000000000000000000000000: nonilhao
1000000000000000000000000000000000: decilhao
1000000000000000000000000000000000000: undecilhao
DICT

run_test "pt: 0"       "zero"          $BINARY "$PT" 0
run_test "pt: 1"       "um"            $BINARY "$PT" 1
run_test "pt: 11"      "onze"          $BINARY "$PT" 11
run_test "pt: 20"      "vinte"         $BINARY "$PT" 20
run_test "pt: 42"      "quarenta dois" $BINARY "$PT" 42
run_test "pt: 100"     "um cem"        $BINARY "$PT" 100
run_test "pt: 1000"    "um mil"        $BINARY "$PT" 1000
run_test "pt: 1000000" "um milhao"     $BINARY "$PT" 1000000
rm -f "$PT"

# ==============================================================================
section "VALOR MODIFICADO (exemplo do subject)"
# ==============================================================================

DM=$(mktemp /tmp/rush02_XXXX.dict)
make_dict "$DM"
sed -i 's/^20: twenty$/20 : hey everybody !/' "$DM"

run_test "20 modificado"          "hey everybody !"       $BINARY "$DM" 20
run_test "21 com 20 modificado"   "hey everybody ! one"   $BINARY "$DM" 21
run_test "outros não afetados"    "forty two"             $BINARY "$DM" 42
rm -f "$DM"

# ==============================================================================
section "ESPAÇOS EXTRAS NO DICT"
# ==============================================================================

DS=$(mktemp /tmp/rush02_XXXX.dict)
cat > "$DS" << 'DICT'
0   : zero
1   : one
2   : two
3   : three
4   : four
5   : five
6   : six
7   : seven
8   : eight
9   : nine
10  : ten
11  : eleven
12  : twelve
13  : thirteen
14  : fourteen
15  : fifteen
16  : sixteen
17  : seventeen
18  : eighteen
19  : nineteen
20  : twenty
30  : thirty
40  : forty
50  : fifty
60  : sixty
70  : seventy
80  : eighty
90  : ninety
100 : hundred
1000: thousand
1000000: million
1000000000: billion
1000000000000: trillion
1000000000000000: quadrillion
1000000000000000000: quintillion
1000000000000000000000: sextillion
1000000000000000000000000: septillion
1000000000000000000000000000: octillion
1000000000000000000000000000000: nonillion
1000000000000000000000000000000000: decillion
1000000000000000000000000000000000000: undecillion
DICT

run_test "espaços antes do colon: 0"    "zero"         $BINARY "$DS" 0
run_test "espaços antes do colon: 42"   "forty two"    $BINARY "$DS" 42
run_test "espaços antes do colon: 1000" "one thousand" $BINARY "$DS" 1000
rm -f "$DS"

# Trim de espaços no final do valor
DT=$(mktemp /tmp/rush02_XXXX.dict)
make_dict "$DT"
sed -i 's/^20: twenty$/20:   twenty   /' "$DT"
run_test "trim espaços no valor: 20" "twenty" $BINARY "$DT" 20
rm -f "$DT"

# ==============================================================================
section "LINHAS VAZIAS NO DICT"
# ==============================================================================

DV=$(mktemp /tmp/rush02_XXXX.dict)
printf '\n0: zero\n\n1: one\n2: two\n3: three\n4: four\n5: five\n' > "$DV"
printf '6: six\n7: seven\n8: eight\n9: nine\n10: ten\n11: eleven\n' >> "$DV"
printf '12: twelve\n13: thirteen\n14: fourteen\n15: fifteen\n' >> "$DV"
printf '16: sixteen\n17: seventeen\n18: eighteen\n19: nineteen\n' >> "$DV"
printf '\n20: twenty\n30: thirty\n40: forty\n50: fifty\n' >> "$DV"
printf '60: sixty\n70: seventy\n80: eighty\n90: ninety\n' >> "$DV"
printf '100: hundred\n1000: thousand\n1000000: million\n' >> "$DV"
printf '1000000000: billion\n1000000000000: trillion\n' >> "$DV"
printf '1000000000000000: quadrillion\n1000000000000000000: quintillion\n' >> "$DV"
printf '1000000000000000000000: sextillion\n' >> "$DV"
printf '1000000000000000000000000: septillion\n' >> "$DV"
printf '1000000000000000000000000000: octillion\n' >> "$DV"
printf '1000000000000000000000000000000: nonillion\n' >> "$DV"
printf '1000000000000000000000000000000000: decillion\n' >> "$DV"
printf '1000000000000000000000000000000000000: undecillion\n\n' >> "$DV"

run_test "linhas vazias: 0"    "zero"         $BINARY "$DV" 0
run_test "linhas vazias: 42"   "forty two"    $BINARY "$DV" 42
run_test "linhas vazias: 1000" "one thousand" $BINARY "$DV" 1000
rm -f "$DV"

# ==============================================================================
section "ENTRADAS EXTRAS NO DICT"
# ==============================================================================

DX=$(mktemp /tmp/rush02_XXXX.dict)
make_dict "$DX" "42: the answer" "1337: leet"

# O programa só faz lookup direto de entradas BASE do dict (0-9, 10-19, 20/30/.../90,
# 100, 1000...). Números compostos como 42 são sempre decompostos em partes.
# Entradas extras servem apenas como novos labels de grupo (chaves do tipo 10^N).
run_test "extra no dict: 42 ainda decompõe"    "forty two"                          $BINARY "$DX" 42
run_test "extra no dict: 1337 ainda decompõe"  "one thousand three hundred thirty seven" \
                                                                                     $BINARY "$DX" 1337
run_test "43 compõe normalmente"               "forty three"                        $BINARY "$DX" 43
rm -f "$DX"

# ==============================================================================
section "DICT ERRORS"
# ==============================================================================

run_test "arquivo inexistente → Error" "Error" \
    $BINARY /tmp/naoexiste_rush02_xyzzy.dict 42

DE=$(mktemp /tmp/rush02_XXXX.dict)
echo "" > "$DE"
run_test "dict vazio → Dict Error" "Dict Error" $BINARY "$DE" 42
rm -f "$DE"

D1=$(mktemp /tmp/rush02_XXXX.dict)
echo "0: zero" > "$D1"
run_test "só 1 entrada → Dict Error" "Dict Error" $BINARY "$D1" 0
rm -f "$D1"

DNO=$(mktemp /tmp/rush02_XXXX.dict)
make_dict "$DNO"
sed -i 's/^0: zero$/0 zero/' "$DNO"
run_test "linha sem colon → Dict Error" "Dict Error" $BINARY "$DNO" 0
rm -f "$DNO"

DVC=$(mktemp /tmp/rush02_XXXX.dict)
make_dict "$DVC"
sed -i 's/^0: zero$/0:/' "$DVC"
run_test "valor vazio após colon → Dict Error" "Dict Error" $BINARY "$DVC" 0
rm -f "$DVC"

DLL=$(mktemp /tmp/rush02_XXXX.dict)
make_dict "$DLL"
sed -i 's/^0: zero$/zero: 0/' "$DLL"
run_test "linha começa com letra → Dict Error" "Dict Error" $BINARY "$DLL" 0
rm -f "$DLL"

# ==============================================================================
section "SAÍDA: sem espaço inicial, sem espaço final"
# ==============================================================================

check_trim()
{
    local n="$1"
    local out
    out=$($BINARY "$n" 2>/dev/null)
    local ok=1
    [[ "$out" == " "* ]] && ok=0
    [[ "$out" == *" " ]] && ok=0
    if [ $ok -eq 1 ]; then
        echo -e "  ${GREEN}PASS${RESET}  $n: sem espaço inicial/final"
        PASS=$((PASS + 1))
    else
        echo -e "  ${RED}FAIL${RESET}  $n: espaço indevido → '${out}'"
        FAIL=$((FAIL + 1))
    fi
}

check_trim 0
check_trim 1
check_trim 20
check_trim 42
check_trim 100
check_trim 1000
check_trim 1000001
check_trim 1000000000

# ==============================================================================
section "SAÍDA: sem espaço duplo no meio"
# ==============================================================================

check_nodouble()
{
    local n="$1"
    local out
    out=$($BINARY "$n" 2>/dev/null)
    if echo "$out" | grep -q "  "; then
        echo -e "  ${RED}FAIL${RESET}  $n: espaço duplo → '${out}'"
        FAIL=$((FAIL + 1))
    else
        echo -e "  ${GREEN}PASS${RESET}  $n: sem espaço duplo"
        PASS=$((PASS + 1))
    fi
}

check_nodouble 100
check_nodouble 1000
check_nodouble 1000000
check_nodouble 1000001
check_nodouble 1001000
check_nodouble 100000

# ==============================================================================
section "SAÍDA: termina com newline"
# ==============================================================================

check_newline()
{
    local n="$1"
    # $() strips trailing newlines, so we append a sentinel char and check
    # that the char before it is \n using printf %q
    local out
    out=$(printf '%s' "$($BINARY "$n" 2>/dev/null)" | tail -c 1 | od -An -tx1 | tr -d ' ')
    if [ "$out" = "0a" ]; then
        echo -e "  ${GREEN}PASS${RESET}  $n: termina com \\n"
        PASS=$((PASS + 1))
    else
        echo -e "  ${RED}FAIL${RESET}  $n: não termina com \\n (último byte: ${out})"
        FAIL=$((FAIL + 1))
    fi
}

check_newline_raw()
{
    local desc="$1"; shift
    local tmpf
    tmpf=$(mktemp)
    "$@" > "$tmpf" 2>/dev/null
    local last
    last=$(tail -c 1 "$tmpf" | od -An -tx1 | tr -d ' \n')
    rm -f "$tmpf"
    if [ "$last" = "0a" ]; then
        echo -e "  ${GREEN}PASS${RESET}  $desc: termina com \\n"
        PASS=$((PASS + 1))
    else
        echo -e "  ${RED}FAIL${RESET}  $desc: não termina com \\n (último byte: '${last}')"
        FAIL=$((FAIL + 1))
    fi
}

check_newline_raw "0"       $BINARY 0
check_newline_raw "42"      $BINARY 42
check_newline_raw "1000000" $BINARY 1000000
check_newline_raw "Error"   $BINARY abc

# ==============================================================================
section "VALGRIND (memory leaks) — requer valgrind"
# ==============================================================================

if command -v valgrind &>/dev/null; then
    vg_check()
    {
        local desc="$1"; shift
        local out
        out=$(valgrind --leak-check=full --error-exitcode=42 "$@" 2>&1)
        local code=$?
        if [ $code -eq 42 ]; then
            echo -e "  ${RED}FAIL${RESET}  valgrind leak: $desc"
            echo "$out" | grep -E "definitely lost|ERROR SUMMARY" | head -3 | sed 's/^/        /'
            FAIL=$((FAIL + 1))
        else
            echo -e "  ${GREEN}PASS${RESET}  valgrind: $desc"
            PASS=$((PASS + 1))
        fi
    }

    VD=$(mktemp /tmp/rush02_XXXX.dict)
    make_dict "$VD"

    vg_check "0"                 $BINARY 0
    vg_check "42"                $BINARY 42
    vg_check "999999"            $BINARY 999999
    vg_check "1000000"           $BINARY 1000000
    vg_check "1000001"           $BINARY 1000001
    vg_check "input inválido"    $BINARY abc
    vg_check "2 args válidos"    $BINARY "$VD" 42
    vg_check "dict vazio"        $BINARY /dev/null 42
    vg_check "arquivo inexist."  $BINARY /tmp/naoexiste_xyzzy.dict 42

    rm -f "$VD"
else
    echo -e "  ${GRAY}SKIP  valgrind não instalado${RESET}"
    SKIP=$((SKIP + 9))
fi

# ==============================================================================
#  RESUMO
# ==============================================================================

TOTAL=$((PASS + FAIL))
echo ""
echo "────────────────────────────────────────────────"
if [ $FAIL -eq 0 ]; then
    echo -e "  ${GREEN}Todos os $TOTAL testes passaram.${RESET}"
else
    echo -e "  ${GREEN}$PASS passaram${RESET}  /  ${RED}$FAIL falharam${RESET}  (total: $TOTAL)"
fi
[ $SKIP -gt 0 ] && echo -e "  ${GRAY}$SKIP ignorados (dependências ausentes)${RESET}"
echo "────────────────────────────────────────────────"
echo ""

[ $FAIL -eq 0 ] && exit 0 || exit 1
