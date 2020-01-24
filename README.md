# Rock-Paper-Scissors

Progetto del corso di Sistemi Operativi, secondo anno della laurea triennale in Informatica presso l'Università degli Studi di Trento

## Studenti

|     #    |                   |                 @                   |
|:--------:|-------------------|------------------------------------:|
| `193868` | Leonardo Berrighi | leonardo.berrighi@studenti.unitn.it |
| `194274` | Alessia Marcolini | alessia.marcolini@studenti.unitn.it |

## Sintesi

Il progetto "rock-paper-scissors" consiste in una simulazione del gioco classico.
Dato in input il numero di concorrenti, genera un campionato andata e ritorno, con parità gestita 'all'italiana'. Successivamente alla creazione della classifica, si determina il vincitore attraverso degli scontri diretti nelle fasi di quarti, semifinali e finale.

Nello specifico, il processo principale genera dei figli per ogni giornata (day), i quali generano a loro volta gli scontri singoli, che consistono in 10 lanci(l'enum determina quale delle tre possibilità) che determinano il vincitore del match. Il tutto è gestito tramite pipe con nome (unidirezionali). Il figlio scrive, il padre legge. L'output è effettuato unicamente dal processo main, al quale vengono passati i dati in blocco dai figli. La generazione degli scontri è supportata da una matrice che ne salva i dati atti per essere utilizzati dal programma. Per lo stesso principio, vengono generate anche una matrice per i risultati di andata, e una per quelli di ritorno. Il turno di riposo è garantito da un valore fittizio nella matrice degli scontri.
Nelle utils sono riportate, come da norma, le funzioni utilizzate. L'attesa di un secondo fra le giornate è necessaria a una buona produzione degli output.
***
Gestione errori
Exit(1) --> valore non intero di input  
Exit(2) --> valore input <8  
Exit(3) --> fork  
Exit(4) --> execv  
Exit(5) --> pipe  
Exit(6) --> malloc  
Exit(7) --> default sullo switch  
Exit(8) --> waitpid  
Exit(9) --> read  
Exit(10) --> dup2  