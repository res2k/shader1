grammar shaderlang2;

options	{
	//backtrack=true;
	k=2;
}

/** \section Programm */

/**

Ein \emph{Programm} besteht aus jeweils keinen bis mehreren Typdefinitionen, Variablen-, Konstanten- und Funktionsdeklarationen.
Der Gültigkeitsbereich dieser Deklarationen ist global.

\label{EOF} Das Terminal \emph{EOF} markiert das Ende der Eingabe.
*/

programm
	:	programm_statements? EOF ;
	
programm_statements
	:	((typ BEZEICHNER)=> funktion_definition | (typ_definition ';') | (dekl_var ';') | (dekl_konst ';')) programm_statements?
	;

/**
\subsection{Eintrittsfunktion}
Der Eintrittspunkt der Ausführung eines Programms ist eine Funktion~(\ref{Funktionen}) namens "`\ident{main}"'. Ist eine solche Funktion nicht definiert, so
kann das Programm nicht ausgeführt werden.

Die Eintrittsfunktion muss den Rückgabetyp \kw{void} besitzen. 

Es müssen genau zwei Ausgabeparameter vom Typ \kw{float4}~(siehe +typ_vektor_float+) deklariert werden.
Der erste Ausgabeparameter nimmt die transformierte und projizierte Position entgegen.
Der zweite Ausgabeparameter nimmt die auszugebende Fragmentfarbe entgegen.

Es können beliebig viele Eingabeparameter beliebigen Typs für die Eintrittsfunktion deklariert werden.
Die Werte dieser Parameter entstammen der Programmumgebung.

*/

/** \section Blöcke

Ein \emph{Block} besteht aus Typdefinitionen, Variablen- und Konstantendeklarationen sowie keinem bis mehreren Kommandos.
Der Gültigkeitsbereich dieser Deklarationen endet mit dem Ende des Blockes. Kommandos werden zur Laufzeit in Reihenfolge ihres
Auftretens ausgeführt.

Ein \kw{return}-Kommando führt zum sofortigen Verlassen der umschließenden Funktion.
Bei Funktionen mit einem anderen Rückgabetyp als \kw{void} muss ein \emph{Ausdruck} mit angegeben
werden, wobei der Rückgabewert der Funktion auf den Wert des Ausdrucks gesetzt wird.
*/

block
	: (((typ_definition | dekl_var ';' | dekl_konst  ';' | (typ '(')=> kommando))) block? ;

kommando
	: ausdruck ';'
	| 'return' ausdruck? ';'
	| verzweigung
	| schleife_for
	| schleife_while
	| ('{' block? '}')
	;

/** \section Ausdrücke 

Der Ausdruck höchster Präzedenz ist die Zuweisung.
%, auf welche einfach in der Regel für
%allgemeine Ausdrücke verwiesen wird.

*/

ausdruck
	: asdr_basis
	| asdr_zuweisung
	| asdr_logisch_oder
	| asdr_ternaer
	;

/** \subsection Basisausdruck

\emph{Basisausdrücke} sind die Ausdrücke niedrigster Präzedenz. Dies sind neben Bezeichnern
für Variablen und Konstanten auch Funktionsaufrufe und geschachtelte Ausdrücke. 
Weiterhin können Zugriffe auf Attribute (siehe +attribut+) bzw. Arrayelemente spezifiziert werden.

 */

asdr_basis
	: (('(' ausdruck ')') | asdr_konst_bool | (typ '(')=> funktion_aufruf | BEZEICHNER | NUMERIC) attribut_liste_oder_array_element?
	;
	
attribut_liste_oder_array_element
	: (('.' attribut) | ('[' ausdruck ']'))  attribut_liste_oder_array_element? ;

/** \subsection Zuweisung

Ein Zuweisungsausdruck setzt sich aus einer "`linken Seite"', einer Variable oder einem Arrayelement
bzw. einem Swizzle-Attribut (siehe \ref{Vektorattribute}) davon, sowie 
einer "`rechten Seite"', dem direkt folgenden, zuzuweisenden +ausdruck+, zusammen.

Dem Ausdruck der linken Seite wird der Wert des Ausdrucks
auf der rechten Seite zugewiesen.

Der Typ des zugewiesenen Ausdrucks muss zuweisungskompatibel (siehe unten) zur linken Seite der Zuweisung sein.

Ein Zuweisungsausdruck selbst hat den Wert der linken Seite nach der Zuweisung.
*/

asdr_zuweisung
	: BEZEICHNER attribut_liste_oder_array_element? '=' ausdruck
	;

/** \subsubsection Zuweisungskompatibilität

Ein Ausdruck ist \emph{zuweisungskompatibel} zu einem bezeichneten Wert wenn 
Wert und Ausdruck den selben Typ besitzen oder eine implizite Typumwandlung
möglich ist.

Ein Ausdruck ist \emph{verlustfrei zuweisungskompatibel} zu einem bezeichneten Wert
wenn Wert und Ausdruck den selben Typ besitzen oder eine implizite Typumwandlung
ohne Präzisionsverlust möglich ist.

Siehe auch \ref{Typumwandlung}.

*/

/**\subsection Binäre Ausdrücke

Ein \emph{binärer Ausdruck} wendet eine arithmetische, bitweise, logische oder vergleichende Verknüpfung
(bestimmt durch den Operator) auf die Werte zweier Ausdrücke an.

Werden zwei Vektoren verknüpft entspricht dies einer Verknüpfung der individuellen Komponenten an der gleichen
Stelle der Vektoren (\attr{x} mit \attr{x}, \attr{y} mit \attr{y}, $\dots$). Zwei verknüpfte Vektoren müssen
die gleiche Komponentenanzahl besitzen.

Bei der Auswertung wird zunächst der linke, dann der rechte Operand ausgewertet, und an\-schließend die Verknüpfung angewendet.
*/

/** \subsection Logische Operatoren

%  "!" - unäres NOT

Beide Operanden müssen vom Typ \kw{bool} sein.

\op{\&\&} logisch UND-verknüpft die Operanden.

\op{||} logisch ODER-verknüpft die Operanden.

*/

asdr_logisch_oder
	: asdr_logisch_und ('||' asdr_logisch_und)* ;
	
asdr_logisch_und
	: asdr_gleichheit ('&&' asdr_gleichheit)* ;

// Bitweise op.?


/** \subsection Vergleichsoperatoren

\op{>} wertet aus ob der linke Operand größer als der rechte Operand ist.

\op{>=} wertet aus ob der linke Operand größer oder gleich dem rechten Operanden ist.

\op{<} wertet aus ob der linke Operand kleiner als der rechte Operand ist.

\op{<=} wertet aus ob der linke Operand kleiner oder gleich dem rechten Operanden ist.

\op{==} wertet aus ob der linke Operand gleich dem rechten Operand ist.

\op{!=} wertet aus ob der linke Operand nicht gleich dem rechten Operanden ist.

Beide Operanden müssen von einem Integer- oder Fließkommatyp (+typ_num+) oder einem
dazu zuweisungskompatiblen Typ sein.

Sind beide Operanden vom Typ \kw{unsigned int} oder \kw{int}, so findet ein Vergleich von Ganzzahlwerten statt.

Ist ein Operand vom Typ \kw{int} und ein Operand vom Typ \kw{int}, \kw{unsigned int} oder
verlustfrei zuweisungskompatibel zu \kw{int} oder \kw{unsigned int}, so findet ein Vergleich von Ganzzahlwerten statt.

Ist ein Operand von einem Fließkommatyp und ein Operand von einem Integer- oder Fließkommatyp
oder zuweisungskompatibel zu dem Fließkommatyp des anderen Operanden,
so findet ein Vergleich von Fließkommawerten statt.
%Die Präzision entspricht der höheren Präzision der beiden Operanden.

*/

/** \alt-merge-on */

asdr_gleichheit
	: asdr_vergleich (('!='|'==') asdr_vergleich)*
	;

asdr_vergleich
	: asdr_add (('>' | '>=' | '<' | '<=') asdr_add)*
	;

/** \alt-merge-off */
	
/**\subsubsection Arithmetische Operatoren

\op{\plus} addiert rechten und linken Operanden.

\op{-} subtrahiert den rechten von dem linken Operanden.

\op{*} multipliziert rechten und linken Operanden.

\op{/} dividiert den linken durch den rechten Operanden.

\op{\%} ist der Divisionsrest der Division des linken Operators durch den rechten Operator.
% TODO Mod bei floats?

Beide Operanden müssen von einem Integer- oder Fließkommatyp (+typ_num+) oder einem
dazu zuweisungskompatiblen Typ sein.

Sind beide Operanden vom Typ \kw{unsigned int}, so ist der Typ des ausgewerteten Ausdrucks \kw{unsigned int}.

Ist ein Operand vom Typ \kw{int} und ein Operand vom Typ \kw{int}, \kw{unsigned int} oder
verlustfrei zuweisungskompatibel zu \kw{int}, so ist der Typ des ausgewerteten Ausdrucks \kw{int}.

Ist ein Operand von einem Fließkommatyp und ein Operand von einem Integer- oder Fließkommatyp
oder zuweisungskompatibel zu dem Fließkommatyp des anderen Operanden,
so ist der Typ des ausgewerteten Ausdrucks von einem Fließkommatyp.
%Die Präzision entspricht der höheren Präzision der beiden Operanden.

*/

/** \alt-merge-on */

asdr_add
	: asdr_mult (('+' | '-') asdr_mult)*
	;
	
asdr_mult
	: asdr_unaer (('*' | '/' | '%') asdr_unaer)*
	;
	
/** \alt-merge-off */

/**\subsection Unäre Ausdrücke

Ein \emph{unärer Ausdruck} transformiert den Wert des Operanden.

Ein unärer Operator, auf einen Vektor angewendet, entspricht der Anwendung auf die individuellen Komponenten.

\op{-} negiert den Operanden. Der Operand muss von einem Integertyp oder Fließkommatyp sein.
Ist der Operand von einem Integertyp, so ist der Typ des unären Ausdrucks \kw{int}.
Ist der Operand von einem Fließkommatyp, so ist der Typ des unären Ausdrucks vom selben Typ.

\op{$\sim$} bitweise invertiert den Operanden. Der Operand muss von einem Integertyp sein. Der Typ des unären Ausdrucks ist vom selben Typ.

\op{!} logisch invertiert den Operanden. Der Operand muss vom Typ \kw{bool} sein. Der Typ des unären Ausdrucks ist vom Typ \kw{bool}.
*/

/** \alt-merge-on */

asdr_unaer
	: ('~'|'-'|'!')? asdr_basis
	;
	
/** \alt-merge-off */
	
/** \subsection Ternärer Ausdruck

Ein ternärer Ausdruck setzt sich aus einem +ausdruck+ (der "`Bedingung"') sowie zwei weiteren
Ausdrücken, dem \emph{Wahr-Ausdruck} und dem \emph{Falsch-Ausdruck}, zusammen.
Die \emph{Bedingung} muss ein boolescher Ausdruck sein. Ergibt sich dieser Ausdruck zu \kw{true},
so wird der \emph{Wahr-Ausdruck} (dem \kw{?} folgend) ausgewertet, und der Wert des ternären
Ausdrucks ergibt sich zu dem Wert des \emph{Wahr-Ausdrucks}.
Ergibt sich die \emph{Bedingung} zu \kw{false}, so wird der \emph{Falsch-Ausdruck} (dem \kw{:} folgend)
ausgewertet, und der Wert des ternären
Ausdrucks ergibt sich zu dem Wert des \emph{Falsch-Ausdrucks}. 

\emph{Wahr-} und \emph{Falsch-Ausdruck} müssen vom gleichen Typ sein.

*/

asdr_ternaer
	: ausdruck '?' ausdruck ':' ausdruck
	;

/**\subsection Boolesche Werte

\kw{true} (wahr) und \kw{false} (unwahr) sind Werte vom Typ \kw{bool} (siehe +typ_bool+).

*/

/** \alt-merge-on */

asdr_konst_bool
	:	'true' | 'false'
	;

/** \alt-merge-off */
	
/** \subsection Attribute

% TODO 'Attr-Vektor', das Swizzle, Komp. ist
% TODO kein Bzch-Attribut sondern Attribute ausschreiben

Einige Variablen (und Konstanten) besitzen über den Wert hinaus weitere Eigenschaften, die 
über Attribute abgefragt werden können. \\
\emph{Attributbezeichner} sind dabei keine reservierten
Schlüsselwörter, d.h. sie können auch als Variablenbezeichner, Funktionsbezeichner etc.
verwendet werden.

\begin{lstlisting}
float4x4 matrix (...);
// Erlaubt:
float4x4 inverted = // 'inverted' hier Variablenbezeichner
  matrix.inverted;  // 'inverted' hier Attribut von 'matrix'
\end{lstlisting}

\noindent Vektorattribute werden in \ref{Vektorattribute} beschrieben.\\
Matrixattribute werden in \ref{Matrixattribute} beschrieben.\\
Arrayattribute werden in \ref{Arrayattribute} beschrieben.\\

*/

attribut
	:	BEZEICHNER
	;

/**\section Typen */

typ_basis
	: typ_num
	| typ_bool
	| typ_vektor
	| typ_matrix
	| typ_sampler
	| BEZEICHNER
	;

typ
	: typ_basis | typ_array
	;

/** \subsection Boolescher Typ

Der Typ \kw{bool} spezifiziert den booleschen Typ mit den möglichen Werten
\kw{true} (wahr) oder \kw{false} (unwahr).

*/

typ_bool
	:	'bool'
	;

/** \subsection Numerische Typen

Der Typ \kw{int} spezifiziert vorzeichenbehaftete Ganzzahlwerte.

Der Typ \kw{unsigned int} spezifiziert vorzeichenlose Ganzzahlwerte.

Genauigkeit und Wertebereich sind jeweils implementierungsabhängig.

Der Typ \kw{float} spezifiziert Fließkommazahlen.

%Optional kann durch +Typ-Float-Präz+ eine gewünschte Präzision +Bzch-Präzision+ angegeben werden.
%Dies kann beeinflussen, in welchem Format und/oder mit welcher Genauigkeit ein
%Wert gespeichert wird.
%+Bzch-Präzision+ ist ein Bezeichner. Vordefinierte Präzisionen sind
%\kw{half}, \kw{single} und \kw{double}. Dies sollte einem Datenformat
%nach IEEE ??? in halber, einfacher und doppelter Genauigkeit entsprechen.

%Ist keine Präzision angegeben soll \kw{single} angenommen werden.

%Unbekannte Präzisionen sind keine Syntaxfehler; Programme mir unbekannten Präzisionen sind gültig.

%Präzision ist Teil des Typs.

%Präzision ist nicht verbindlich. Genauigkeit und Wertebereich sind jeweils implementierungsabhängig.

*/

typ_num
	:	(('unsigned')? 'int')
	|	'float'
	;
	
/** \subsection Vektortypen

% "normalized"?


Der \emph{Basistyp} eines Vektors ist ein Integer-, Boolescher oder Fließkommatyp. 
Die Zahl $N$ nach dem Basistyp ist vom Typ
Ganzzahl, kann Werte zwischen 1 und 4 inklusive annehmen, und gibt die Komponentenanzahl an.
 Ein Vektortyp spezifiziert ein $N$-Tupel von Werten 
des Basistyps.
*/

typ_vektor
	:	typ_vektor_int
	|	typ_vektor_float
	|	typ_vektor_bool
	;
/** \alt-merge-on */
	
	
typ_vektor_int
	:	(('unsigned')? ('int1' | 'int2' | 'int3' | 'int4'))
	;
	
typ_vektor_float
	:	('float1' | 'float2' | 'float3' | 'float4')
	;
	
typ_vektor_bool
	:	('bool1' | 'bool2' | 'bool3' | 'bool4')
	;
	
/** \alt-merge-off */
	
/*attr_vektor
	:	SWIZZLE_RGBA | SWIZZLE_XYZW
	;*/
	

/** \subsubsection Vektorkonstruktoren

Vektorkonstruktoren sind besondere, vorgegebene Funktionen, die gleiche Bezeichner wie Vektortypen haben
(siehe auch +funktion_aufruf+).

Wird ein Vektor aus \emph{einem} Basisausdruck konstruiert, so haben alle Komponenten diesen Wert.
Der \emph{Basisausdruck} muss als Typ einen Basistyp besitzen.

Wird ein Vektor aus \emph{mehreren} Basis- oder Vektorausdrücken oder einem Vektorausdruck konstruiert,
so wird der ersten Komponente der Wert des ersten Basisausdrucks bzw. der ersten Vektorausdruckskomponente zugewiesen,
der zweiten Komponente der Wert des nächsten Basisausdrucks bzw. der nächsten Vektorausdruckskomponente usw.
Allen Komponenten muss ein Wert zugewiesen werden. Die Anzahl aller Basisausdrücke und Komponenten der Vektorausdrücke
zusammen muss exakt der Komponentenanzahl entsprechen.
Alle verwendeten Vektortypen müssen vom gleichen Basistyp wie der zu konstruierende Vektor sein.

\begin{lstlisting}[basicstyle=\ttfamily\small,lineskip=-11pt]
float3 a = float3 (1.0);           // a = (1.0, 1.0, 1.0)
float3 b = float3 (1.0, 2.0, 3.0);
float4 c = float4 (b, 0.5);        // c = (1.0, 2.0, 3.0, 0.5)
float3 d = float3 (1.0, 2.0);      // Fehler
				   // (nicht alle Komponenten gegeben)
\end{lstlisting}

*/

/**
% Besserer Platz?

\subsubsection Vektorattribute
Einzelne Komponenten können entweder mit Array-Syntax oder den Attributen \attr{x},
\attr{y}, \attr{z}, \attr{w} bzw. \attr{r}, \attr{g}, \attr{b}, \attr{a} angesprochen
werden. \attr{x}/\attr{r} spezifiert die erste Komponente, 
\attr{y}/\attr{g} die zweite Komponente, \attr{z}/\attr{b} die dritte Komponente, 
\attr{w}/\attr{a} die vierte Komponente. Ansprechen einer nicht existierenden Komponente
führt zu einem Fehler.

Weitere Attribute sind die "`Swizzle"'-Attribute (+SWIZZLE_RGBA+, +SWIZZLE_XYZW+).
Das Ergebnis eines Swizzles ist vom Typ eines Vektors des Basistyps, mit der Anzahl der Komponenten
entsprechend der Anzahl der Komponenten im "`Swizzle"'. Die erste Komponente des Wertes entspricht
der Komponente des Ursprungsvektors, die an erster Stelle des Swizzles identifiziert wird, usw.

\begin{lstlisting}[basicstyle=\ttfamily\small,lineskip=-11pt]
float4 sampleVec = float4 (1.0, 2.0, 3.0, 4.0);
float  a = sampleVec.x;    // a = 1.0
float2 b = sampleVec.wz;   // b = (4.0, 3.0)
float4 c = sampleVec.xxxx; // c = (1.0, 1.0, 1.0, 1.0)
float4 d = sampleVec.zwyy; // d = (3.0, 4.0, 2.0, 2.0)
float4 e = sampleVec.bgra; // e = (3.0, 2.0, 1.0, 4.0)
float4 f = sampleVec.xyba; // Fehler (XYZW/RGBA gemischt)
float  g = b.z;            // Fehler (Komponente existiert nicht)
\end{lstlisting}

*/

/** \subsection Matrixtypen

Der \emph{Basistyp} einer Matrix ist ein Integer-, Boolescher Typ oder Fließkommatyp. 
Dem Basistyp folgt eine Zeichenkette der Form $N$\kw{x}$M$. $N$ und
$M$ sind vom Typ Ganzzahl, können jeweils Werte zwischen 1 und 4 inklusive annehmen,
und geben die Spalten- und Zeilenzahl
der Matrix an. Ein Matrixtyp spezifiziert eine  $N \times M$-Matrix von Werten des Basistyps.
*/

typ_matrix
	:	typ_matrix_float
	|	typ_matrix_int
	|	typ_matrix_bool
	;
	
/** \alt-merge-on */

typ_matrix_float
	:	'float1x1' | 'float2x1' | 'float3x1' | 'float4x1' |
		'float1x2' | 'float2x2' | 'float3x2' | 'float4x2' |
		'float1x3' | 'float2x3' | 'float3x3' | 'float4x3' |
		'float1x4' | 'float2x4' | 'float3x4' | 'float4x4'
	;

typ_matrix_int
	:	('unsigned')? ('int1x1' | 'int2x1' | 'int3x1' | 'int4x1' |
		'int1x2' | 'int2x2' | 'int3x2' | 'int4x2' |
		'int1x3' | 'int2x3' | 'int3x3' | 'int4x3' |
		'int1x4' | 'int2x4' | 'int3x4' | 'int4x4')
	;

typ_matrix_bool
	:	'bool1x1' | 'bool2x1' | 'bool3x1' | 'bool4x1' |
		'bool1x2' | 'bool2x2' | 'bool3x2' | 'bool4x2' |
		'bool1x3' | 'bool2x3' | 'bool3x3' | 'bool4x3' |
		'bool1x4' | 'bool2x4' | 'bool3x4' | 'bool4x4'
	;

/** \alt-merge-off */

/** \subsubsection Matrixkonstruktoren

Matrixkonstruktoren sind besondere, vorgegebene Funktionen die gleiche Bezeichner wie Matrixtypen haben
(siehe auch +funktion_aufruf+).

%Wird eine Matrix aus einem Matrix-Ausdruck konstruiert so haben alle Felder der Matrix die
%Werte der entsprechenden Quellmatrix. Die Dimensionen der Quellmatrix muss denen von
%+Typ-Matrix+ entsprechen.
%+Asdr-Basis+ muss als Typ einen Matrix-Typ besitzen.

Eine Matrix wird aus einem oder mehreren Vektorausdrücken zeilenweise konstruiert. Den Zeilen werden
jeweils die Werte der Vektorausdrücke zugewiesen. Die Komponentenanzahl der Vektoren muss dabei
$N$ sein. Die Anzahl der gegebenen Vektoren muss $M$ sein.

%Wird eine Matrix aus einem oder mehreren Basisausdrücken konstruiert so werden den Feldern
%nacheinander jeweils die Werte dieser Ausdrücke zugewiesen. Den Feldern der ersten Zeile werden
%die ersten +N+ Werte zugewiesen, den Feldern der zweiten Zeile die nächsten +N+ Werte
%usw. Die Anzahl der gegebenen Werten muss +N+$\cdot$+M+ sein.

\begin{lstlisting}
int2 v1 = int2 (1, 2);
int2 v2 = int2 (3, 4);
int2x2 m1 = int2x2 (v1, v2); // m1 = (1, 2, 3, 4);
int3x2 m2 = int3x2 (v1, v2); // Fehler: v1, v2 besitzen
			     // nicht 3 Komponenten
int3x2 m3 = int3x2 (int3 (v1, 0),  // m3 = (1, 2, 0,
                    int3 (v2, 0)); //       3, 4, 0)
\end{lstlisting}


*/

/** \subsubsection Matrixattribute

Einzelne Zeilen können über das Attribute \attr{row} angesprochen werden. Dieses
verhält sich wie ein Array mit $M$ Elementen des Vektortyps \emph{Basistyp}$N$.

Einzelne Spalten können über das Attribut \attr{col} angesprochen werden. Dieses
verhält sich wie ein Array mit $N$ Elementen des \emph{Basistyp}$M$.

Das Attribut \attr{transposed} ist eine Matrix vom Typ \emph{Basistyp}$M$x$N$
und hat als Wert die transponierte Ursprungsmatrix.

Matrizen mit gleichem $N$ und $M$ besitzen das Attribut \attr{inverted}.
Es ist eine Matrix vom Typ \emph{Basistyp}$N$x$M$ und hat als Wert die invertierte Ursprungsmatrix. 
Das Ergebnis ist undefiniert, wenn die Ursprungsmatrix nicht invertierbar ist.
\begin{lstlisting}
float2x2 m1 = float2x2 (float2 (0, 2),
		        float2 (0.2, 0));
int2 r1 = m1.row (1);       // r1 = (0.2, 0)
int2 c1 = m1.col (1);       // c1 = (2, 0)
float2x2 t = m1.transposed; // t = (0, 0.2, 2, 0)
float2x2 i = m1.inverted;   // i = (0, 0.5, 5, 0)
\end{lstlisting}
*/

/*attr_matrix
	:	'row'
	|	'col'
	|	'transposed'
	|	'inverted'
	;*/
	

/** \subsection Samplertypen */
 
/** \alt-merge-on */

typ_sampler
	: 'sampler1D'
	| 'sampler2D'
	| 'sampler3D'
	| 'samplerCUBE'
	;
	
/** \alt-merge-off */
	
/**
Samplertypen repräsentieren Textureinheiten der Hardware. Texturen verschiedener
Dimensionalität müssen in Shadingprogrammen verschiedenartig angesprochen werden;
dies bedingt die mehrfachen Samplertypen, die jeweils einen speziellen Texturtyp
reflektieren.

Texturen werden über spezielle vordefinierte Funktionen ausgelesen (siehe~\ref{Texturfunktionen}).

 */

/** \subsection Arraytypen

Der Typ der Elemente im Arraytyp wird durch den \emph{Basistyp} spezifiziert (siehe auch +typ+;
insbesondere ist der Basistyp nicht auf +typ_basis+ beschränkt sondern kann selbst ein Arraytyp sein).
Die Anzahl der Elemente in individuellen Variablen oder Konstanten eines Arraytyps
ergibt sich aus der Anzahl der Elemente des zugewiesenen Array-Wertes.

Elemente einer Array-Variable oder -Konstante können mit \ident{Bezeichner[}$\mathit{Index}$\ident{]} angesprochen
werden. Das erste Element wird mit dem Wert $0$ für $\mathit{Index}$ angesprochen. 
Liegt $\mathit{Index}$ ausserhalb des gültigen Bereiches ($0\dots{}N-1$) so ist der ausgelesene Wert
undefiniert.

*/

typ_array
	: typ '[' ']'
	;
/**
\subsubsection Arraykonstruktoren

%Ktor-Array:
%  Bzch-Typ."[]" "(" [Ausdruck { "," Ausdruck } "," ] ")"

Arraykonstruktoren sind besondere, vorgegebene Funktionen die gleiche Bezeichner wie Arraytypen haben
(siehe auch +funktion_aufruf+).

Ein Array wird aus keinem, einem oder mehreren Werten des Elementtyps konstruiert.
Die Anzahl der Elemente entspricht der Anzahl der dem Konstruktor übergebenen Ausdrücke.

\begin{lstlisting}
int[] a = int[] (1, 2, 3); // 'a' besitzt 3 Elemente
int e1 = a[1];             // e1 = 2
int e2 = a[4];             // e2 ist undefiniert
\end{lstlisting}
*/

/** \subsubsection Arrayattribute
	
Arrays besitzen ein Attribut \attr{length} vom Typ \kw{unsigned int} dessen Wert
die Anzahl der Elemente im Array ist.

\begin{lstlisting}
int[] a = int[] (1, 2, 3);
unsigned int l = a.length; // l = 3
\end{lstlisting}
*/

/*attr_array
	:	'length'
	;*/
	

/** \subsection Typdefinitionen

+typ_definition+ deklariert einen neuen Typ mit dem angegeben Bezeichner der ein Alias für den durch \glq\Gnt{typ}\grq\ bezeichneten Typen ist.

Die Sichtbarkeit des Typbezeichners beginnt hinter der +typ_definition+
und reicht bis zum Ende des Gültigkeitsbereichs in dem die Definition vorgenommen wurde.

Der Bezeichner darf keinen anderen Bezeichner der umgebenden Gültigkeitsbereiche überdecken.

*/

typ_definition
	: 'typedef' typ BEZEICHNER
	;

/** \subsection Typumwandlung

\subsubsection{Implizite Typumwandlungen}
Ausdrücke eines numerischen Typs können in einen anderen numerischen Typ umgewandelt werden.
Wird ein Ausdruck einen numerischen Typs einer Variable oder einem formalen Funktionsparameter
eines anderen numerischen Typs zugewiesen, findet eine \emph{implizite} Typumwandlung statt
(Zuweisungskompatibilität).

Ist \emph{verlustfreie} Zuweisungskompatibilität verlangt, wird eine implizite Typumwandlung nur
vorgenommen, wenn kein Präzisionsverlust auftritt (nach nachfolgenden Definitionen). Tritt ein
Präzisionsverlust auf, ist das Programm ungültig.

\kw{int} und \kw{unsigned int} werden als immer untereinander ohne Präzisionsverlust zuweisbar
angenommen.

Eine Zuweisung von \kw{int} oder \kw{unsigned int} an \kw{float} wird immer als ohne Präzisionsverlust zuweisbar
angenommen.

Eine Zuweisung von \kw{float} zu \kw{int} oder \kw{unsigned int} wird immer mit Präzisionsverlust zuweisbar angenommen.

\subsubsection{Explizite Typumwandlungen}
Ausdrücke können explizit in einen anderen Typ umgewandelt werden (syntaktisch ein Funktionsaufruf mit
Typbezeichner als Funktionsbezeichner). Umwandlungen mit Präzisionsverlust sind dabei erlaubt.
Ein Ausdruck einer expliziten Typumwandlung hat den Typ in den explizit umgewandelt wurde.

*/

	
/** \section Funktionen

\subsection Definition

+funktion_definition+ deklariert eine Funktion mit dem gegebenen Bezeichner.

Die Sichtbarkeit des Funktionsbezeichners beginnt hinter dem \Gt{)} von +funktion_definition+
und reicht bis zum Ende des Programms.

Der Bezeichner darf keinen anderen Bezeichner des globalen Gültigkeitsbereiches überdecken.
Ausnahme: Mehrere Funktionen können den gleichen Bezeicher besitzen; es handelt sich hierbei
um \emph{überladene} Funktionen (siehe~\ref{Ueberladene Funktionen}).

+funktion_typ+ spezifiziert den Typ des Rückgabewertes der Funktion. Der spezielle Typ \kw{void}
gibt an, dass die Funktion keinen Wert zurückgibt.
%\kw{void} ist weder von, noch nach, irgendeinen anderen Typ umwandelbar.
Weder ist der Typ \kw{void} in irgendeinen anderen Typ umwandelbar, noch ist eine Umwandlung
von irgendeinem Typen nach \kw{void} möglich.

+funktion_param_formal+ beschreibt die Parameter, welche die Funktion annimmt.
Parameter können Eingabe- und/oder Ausgabeparameter sein: auf \emph{Eingabeparameter} kann
nur lesend zugegriffen werden, ein Schreibzugriff ist nicht erlaubt.
\emph{Ausgabeparameter} können beschrieben werden und jede Änderung wirkt sich auch sofort auf
den zugeordneten aktuellen Parameter aus. Ausgabeparameter können auch gelesen werden,
allerdings ist ihr Wert vor dem ersten Beschreiben undefiniert.
\emph{Ein- und Ausgabeparameter} können jederzeit gelesen werden, jeder Schreibzugriff wirkt sich
aber auch hier auf den zugeordneten aktuellen Parameter aus.
  
*/

funktion_definition
	:	funktion_kopf '{' block '}'
	;
	
funktion_kopf
	:	funktion_typ BEZEICHNER '(' funktion_param_formal_liste? ')'
	;
	
funktion_typ
	: typ
	| 'void'
	;
	
funktion_param_formal_liste
	: funktion_param_formal (',' funktion_param_formal)*
	;
	
funktion_param_formal
	: 'in'? 'out'? typ BEZEICHNER ('=' ausdruck)?
	;

/**
\subsection Aufruf

Funktionen werden mit +funktion_aufruf+ aufgerufen. Zunächst werden die
gegebenen Werteparameter(+funktion_param_aktuell+) den Funktionsparametern der Funktion über ihre Stelle zugeordnet. 
Basierend auf der Anzahl der Parameter und den Typen der Parameterausdrücke wird eine Überladung der Funktion ausgewählt.
Den Funktionsparametern werden die entsprechenden Werte der Parameterausdrücke zugewiesen.
Die Parameterausdrücke werden von links nach rechts ausgewertet.
Die Programmausführung springt dann zu dem ersten
Kommando des Funktionsblockes. Nach Beendigung von dessen Ausführung wird dem Wert des Aufrufausdrucks der
Rückgabewert der Funktion zugewiesen. Die Programmausführung fährt mit dem ursprünglichen Funktionsaufruf
unmittelbar folgendem Kommando fort. 

Als Bezeichner für die aufzurufende Funktion können nicht nur Bezeichner benutzerdefinierter und
vordefinierter Funktionen sondern auch \emph{Typbezeichner} dienen.
Im Fall von Vektor-, Matrix- oder Arraytypen sind dies die speziellen Konstruktoren
(siehe \ref{Vektorkonstruktoren}, \ref{Matrixkonstruktoren} und \ref{Arraykonstruktoren}).
Im Fall von numerischen Typen wird eine explizite Typumwandlung (siehe 
\ref{Typumwandlung}) veranlasst.

*/

funktion_aufruf
	: (typ) '(' funktion_param_aktuell? ')'
	;
	
funktion_param_aktuell
	: ausdruck (',' ausdruck)*
	;

/** \subsection Überladene Funktionen

Es können mehrere Funktionen mit identischem Bezeichner, aber unterschiedlicher Signatur,
deklariert werden. Die Signatur einer Funktion wird aus Position und Typ jedes Parameters
bestimmt. Werden zwei Funktionen mit identischem Bezeichner und Signatur deklariert ist
das Programm ungültig.

Die aufzurufende Überladung einer Funktion wird über die Signatur bestimmt. Existiert
eine Überladung, deren Signatur genau mit den übergebenen aktuellen Parametern
übereinstimmt wird diese ausgewählt. Andernfalls werden aus den Überladungen mit
gleicher Parameteranzahl, aber unterschiedlichen Typen "`Kandidaten"' für den
Aufruf ausgewählt. Eine Funktion ist "`Kandidat"', wenn an jeder Stelle der Signatur
der Typ identisch mit dem übergebenen Typ oder in diesen verlustfrei umwandelbar ist.
Gibt es genau einen Kandidaten wird dieser ausgewählt und die Programmausführung
mit diesem fortgesetzt. Gibt es keinen oder mehrere Kandidaten ist das Programm
ungültig.

 */

/** \section Konstanten und Variablen */

/** \subsection Variablendeklarationen

+dekl_var+ deklariert eine oder mehrere neue Variablen des gegebenen Typs und den gegebenen Bezeichnern.

Die Sichtbarkeit der Bezeichner beginnt nach der jeweiligen Deklaration 
und reicht bis zum Ende des umgebenden Blockes.

Der Bezeichner darf keinen anderen Bezeichner des umgebenden Gültigkeitsbereiches überdecken.

Der Wert von einer deklarierten Variable ist anfänglich undefiniert.

Wird ein initialer Ausdruck angegeben, so entspricht dies semantisch einer Zuweisungsoperation
hinter +dekl_var_bzch_init+, aber vor der Deklaration der nächsten Variable.
Der Typ des initialen Ausdrucks muss zuweisungskompatibel zum Typ der Variable sein.
*/

dekl_var
	: typ dekl_var_bzch_init (',' dekl_var_bzch_init)*
	;
	
dekl_var_bzch_init
	: BEZEICHNER ('=' ausdruck)?
	;
	
/** \subsection Konstantendeklarationen

+dekl_konst+ deklariert eine oder mehrere neue Konstanten des gegebenen Typs und den gegebenen Bezeichnern.

Die Sichtbarkeit der Bezeichner beginnt nach der jeweiligen Deklaration
und reicht bis zum Ende des umgebenden Blockes.

Der Bezeichner darf keinen anderen Bezeichner des umgebenden Gültigkeitsbereiches überdecken.

Die Konstante wird mit dem Wert des initialen Ausdrucks initialisiert.
Der Typ des initialen Ausdrucks muss zuweisungskompatibel zum Typ der Konstante sein.
*/

dekl_konst
	: 'const' typ dekl_konst_bzch_init (',' dekl_konst_bzch_init)*
	;
	
dekl_konst_bzch_init
	: BEZEICHNER '=' ausdruck
	;
	
/** \section Ablaufsteuerung */


/** \subsection Verzweigungen

\glq\Gnt{ausdruck}\grq{} ist ein boolescher Ausdruck, die \emph{Bedingung}.

Wenn die Bedingung zu \kw{true} ausgewertet wird, werden die Kommandos des ersten Blockes (\emph{Wahr-Block}) ausgeführt,
ansonsten die Kommandos des Blockes im \kw{else}-Zweig (\emph{Falsch-Block}).

Der \kw{else}-Zweig ist optional.

%Die Berechnungsfrequenz der Verzweigung ist die kleinste gemeinsame Frequenz der Blöcke sowie der Bedingung.
*/

verzweigung
	:	'if' '(' ausdruck ')' '{' block '}' ('else' '{' block '}')?
	;
	
/** \subsection \kw{for}-Schleifen

Der erste Ausdruck ist die \emph{Initialisierung}. Der zweite Ausdruck ist die \emph{Schleifenbedingung}. Der dritte Ausdruck ist der \emph{Zählausdruck}.

Bei der Ausführung wird zunächst die \emph{Initialisierung} ausgeführt.

Die \emph{Schleifenbedingung} wird ausgewertet. Ist das Ergebnis \kw{true}, so werden die Kommandos des Schleifenblockes
und anschließend der \emph{Zählausdruck} ausgeführt. 
Dieser Ablauf wird wiederholt bis eine Auswertung der \emph{Schleifenbedingung} das Ergebnis \kw{false} hat.

%Die Berechnungsfrequenz der Schleife ist die kleinste gemeinsame Frequenz des Schleifenblockes sowie 
%der \emph{Initialisierung}, der \emph{Schleifenbedingung} und des \emph{Zählausdrucks}.
*/

schleife_for
	:	'for' '(' ausdruck? ';' ausdruck? ';' ausdruck? ')' '{' block '}'
	;

/** \subsection \kw{while}-Schleifen

\glq\Gnt{ausdruck}\grq ist ein boolescher Ausdruck, die \emph{Bedingung}.

Die Bedingung wird ausgewertet. Ist das Ergebnis \kw{true}, so werden die Kommandos des Schleifenblocks
ausgeführt. Dieser Ablauf wird wiederholt bis eine Auswertung der Bedingung das Ergebnis \kw{false} hat.

%Die Berechnungsfrequenz der Schleife ist die kleinste gemeinsame Frequenz des Schleifenblockes sowie der Bedingung.
*/

schleife_while
	:	'while' '(' ausdruck ')' '{' block '}'
	;

//  Ktor-Vektor
//  Ktor-Matrix
//  Ktor-Array
//  Asdr-Attribut-Zugriff
//  Typumwandlung
//  Aufruf-Funktion

/** \section Lexikalische Einheiten

\emph{Anmerkung:} Da diese Regeln die lexikalischen Einheiten selbst beschreiben gibt es
hier \emph{keine} weiteren Trennzeichen zwischen Terminalen!	

*/

/** \subsection Bezeichner

\emph{Bezeicher} ergeben sich aus ein oder mehreren Buchstaben, Ziffern und dem Zeichen \Gt{_}. Das erste Zeichen
darf keine Ziffer sein.

Der Unterteilung in Buchstaben und Ziffern liegen Unicode-Kategorien zugrunde (siehe~\cite{unicode}, Abschn. 4.5).
% TODO Ref: Unicode-Standard - 5.1.0 4.1

Zwei Bezeichner sind identisch wenn deren Zeichensequenzen \emph{kanonisch äquivalent} nach Unicode sind (siehe~\cite{unicode}, Kap. 3, Def. D70)
\footnote{Verschiedene Glyphen können auf verschiedene Weisen in Unicode codiert werden: z.B. \emph{Ä} 
mit einem einzelnen Zeichen (\texttt{U\plus{}00C4} ``Latin capital letter A with diaresis'') oder als Kombination
von zwei Zeichen (\texttt{U\plus{}0041} ``Latin capital letter A'' und \texttt{U\plus{}0308} ``Combining diaresis'').
Bei einem einfachen Stringvergleich würden diese beiden Darstellungen als verschieden angesehen --
für einen menschlichen Leser wären sie aber semantisch äquivalent.
Die Unicode-Regeln für kanonische Äquivalenz legen fest, wie solche Glyphen
dargestellt werden sollen, damit aus semantisch äquivalente Zeichenketten auch "`binär"' gleiche
Zeichenketten resultieren.}.
% Unicode 5.1.0 2.12

Lexikalische Einheiten können mit einem oder mehreren Zeichen der Unicode-Kategorie "`Separator"' getrennt werden.

*/

/** \alt-merge-on */

/** \customrule Number Unicode-Ziffer */
fragment Number :   '0'..'9' ;
/** \customrule Letter Unicode-Buchstabe */
fragment Letter
	:	('a'..'z'|'A'..'Z') ;
fragment Comp_XYZW : 'x'|'y'|'z'|'w';
fragment Comp_RGBA : 'r'|'g'|'b'|'a';
SWIZZLE_XYZW 
	:	 Comp_XYZW (Comp_XYZW (Comp_XYZW (Comp_XYZW)?)?)?;
SWIZZLE_RGBA
	:	 Comp_RGBA (Comp_RGBA (Comp_RGBA (Comp_RGBA)?)?)?;
BEZEICHNER  : (Letter|'_') (Letter|Number|'_')* ;
/** \customrule Whitespace Unicode-Separator */
fragment Whitespace : (' '|'\t'|'\r'|'\n') ;
WS  :  Whitespace WS? { $channel = HIDDEN; } ;

/** \subsection Numerische Literale

Ganzzahlen können in Dezimal- und Hexadezimalnotation angegeben werden.

Fließkommazahlen können als Dezimalbruch oder in Exponentialschreibweise
angegeben werden.

 */

fragment Digit_Dec : '0'..'9' ;
fragment Digits_Dec : Digit_Dec Digits_Dec? ;
fragment Digits_Dec_Sign : '-'? Digits_Dec ;
fragment Digits_Dec_Frac : '.' Digits_Dec ;
fragment Num_Float_Exp : (('e'|'E') Digits_Dec_Sign) ;
fragment Num_Float_Mantissa : (Digits_Dec ('.' | Digits_Dec_Frac)) | (Digits_Dec_Frac) ; 
fragment Num_Float	:	Num_Float_Mantissa Num_Float_Exp? ;
fragment Digit_Hex : (Digit_Dec|'A'..'F'|'a'..'f');
fragment Digits_Hex : Digit_Hex Digits_Hex? ;
fragment Num_Hex	:	'0x' Digits_Hex ;
NUMERIC :	Num_Float | Num_Hex;

/** \subsection Kommentare

 +COMMENT+ ist ein einzeiliger Kommentar. Alle Eingabezeichen bis zum nächsten Zeilenumbruch
 werden ignoriert.
 
 +ML_COMMENT+ ist ein mehrzeiliger Kommentar. Alle Eingabezeichen, inklusive Zeilenumbrüchen,
 zwischen Start- (\kwMlcStart) und nächster Endmarkierung (\kwMlcEnd) werden ignoriert.
 */

/** \customrule COMMENT \Gt{//} beliebige Zeichen \Gspace\Galt{\Gt{$\backslash$r}\Gor{}\Gt{$\backslash$n}} */
COMMENT
	: '//' .* ('\r'|'\n') {$channel=HIDDEN;}
	;
/** \customrule ML_COMMENT \Gt{\kwMlcStart} beliebige Zeichen \Gt{\kwMlcEnd} */
ML_COMMENT
	: '/*' (options {greedy=false;} : .)* '*/' {$channel=HIDDEN;}
    ;
