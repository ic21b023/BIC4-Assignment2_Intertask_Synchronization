Ohne Semaphoren werden die Task beim schreiben und lesen unterbrochen wodurch die Daten nicht richtig verarbeitet und
ausgegeben werden.

Mithilfe von Semaphoren wird der Zugriff der Daten kontrolliert und daher werden die Daten richtig verarbeitet und 
ausgegeben.

p.s.
Für das Testen ohne semaphoren mitt NOT_CRITICALLY_SAFE im main.c definiert werden
//# define NOT_CRITICALLY_SAFE	