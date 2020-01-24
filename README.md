# Rock-Paper-Scissors

Progetto del corso di Sistemi Operativi, secondo anno della laurea triennale in Informatica presso l'Università degli Studi di Trento

## Studenti

|     #    |                   |                 @                   |
|:--------:|-------------------|------------------------------------:|
| `193868` | Leonardo Berrighi | leonardo.berrighi@studenti.unitn.it |
| `194274` | Alessia Marcolini | alessia.marcolini@studenti.unitn.it |

## Sintesi

Il progetto "rock-paper-scissors" consiste in una simulazione del gioco classico.
Dato in input il numero di concorrenti (che deve essere maggiore o uguale a 8), genera un campionato andata e ritorno, con parità gestita 'all'italiana'. Successivamente alla creazione della classifica, si determina il vincitore attraverso degli scontri diretti nelle fasi di quarti, semifinali e finale.

Nello specifico, il processo principale genera dei processi figli per ogni giornata (day), i quali generano a loro volta gli scontri singoli, che consistono in 10 lanci (con possibile risultato deò lancio `rock`, `paper`, `scissors`) che determinano il vincitore del match. Il tutto è gestito tramite pipe anonime (unidirezionali). Il figlio scrive, il padre legge. Il feedback a schermo è effettuato unicamente dal processo main, al quale vengono passati i dati in blocco dai figli. La generazione degli scontri è supportata da una matrice che ne salva i dati atti per essere utilizzati dal programma. Per lo stesso principio, vengono generate anche una matrice per i risultati di andata, e una per quelli di ritorno. Il turno di riposo è garantito da un valore fittizio nella matrice degli scontri.

Codici di errore:
* 1 --> valore non intero di input
* 2 --> valore input < 8
* 3 --> fork error
* 4 --> execv error
* 5 --> pipe error
* 6 --> malloc error
* 7 --> match error
* 8 --> waitpid error
* 9 --> read pipe error
* 10 --> dup2 error