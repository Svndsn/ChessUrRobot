ChessMoveDetector

------------------
Setup:

For at bruge ChessMoveDetector skal du sætte kameraet op, så det kun indeholde skakbrættets felter.

Dette gøres desværre manuelt ved at se på det billede den tager og cropper det ved at lave en Rect roi(x,y, width, height)

Denne kaldes så på billedet image = image(roi) - Da et skakbræt er kvadratisk skal height og width være det samme.

Der udover kan der være en rotation i billedet.

For at se om skakbrættets felter passer med hvad klassen mener der er felterne, kan showGrid(image) kaldes for at tegne de felter ind som klassen tror de er. 

For at klassen kan detecte moves skal brættet stå, så hvid er i venstre side og sort i højre.

Dette skal dog desværre gøres inde i cppfilen, her er der to steder hvor der er en rotate(), hvor man kan rotere billedet
linje 18 og 116 //Denne del burde nok være sin egen funktion eller et del af klassens input

Hvis man bruger et ekstern kamera, kan man være nødt til at gå ind i på takePicture() og ændre tallet i VideoCapture cam(0, CAP_V4L2);

------------------
Detection:

Når man har sat det op, er det bare at bruge detectMove(fen) og give den et fen som input. 
Den tage baseImage som er den første reference inden der bliver rykket, når klassen kaldes.


------------------

Hvordan selve funktionen fungerer:

Den finder forskellene i de to billeder. Her finder den så midtpunktet for de 2 største forskelle. 

Punkterne sammenlignes med hvor de befinder sig i billedet, og da billedet kun indeholder skakbrættet, svarer der position i billedet til positionen på brættet

Den har nu 2 koordinatsæt, men skal finde frem til hvordan der er blevet rykket. 

Det gør den ved at tage den inputtede fenstring og lave den til matrice som indeholder hvor der er sorte og hvide, og hvem der har turen

Den information bruger den så til at finde ud af hvor der blev rykket fra og hvor der blev rykket til.

Dette konverterer den så til skakfelter i en string med trækket.

Den sætter så det nye billede til det der skal være reference punkt for næste

