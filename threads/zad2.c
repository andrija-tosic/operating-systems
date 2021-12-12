/*
Koriscenjem programskog jezika C kreirati program koji se deli u dva Linux procesa koji generisu slucajne
celobrojne vrednosti i upisuju ih u datoteku ciji se naziv prosledjuje kao argument komandne linije glavnog procesa.
Prvi proces generise niz od tri slucajne pozitivne celobrojne vrednosti i upisuje ih u datoteku.
Nakon toga drugi proces generise dve slucajne negativne celobrojne vrendosti i upisuje ih u istu datoteku.
Postupak se ponavlja N puta (naizmenicno prvi pa drugi proces).
Vrednost N se takodje prosledjuje kao argument komandne linije. Sinhronizovati rad procesa koriscenjem semafora.
*/

