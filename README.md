Réalisation du cable
-----------

En premier lieu, nous avons soudé un connecteur [DIN 45° mâle](http://en.wikipedia.org/wiki/DIN_connector)
À un câble téléphonique de minitel, que nous avons soudé à des pins, voici le résultat.

Branchement de l'arduino
-----------

Ici, j'ai utilisé un arduino uno.
J'ai branché le fil rouge à ground, et les deux autres fils aux pins 6 et 7.

Présentation de mon Makefile
-----------
Je préfère utiliser mes propres (gnu)Makefiles qu'utiliser directement l'IDE arduino.
J'utilise pour tous mes projets un makefile racine, que j'inclue dans mes projets.
J'explique cela sur le [github arduino makefile](https://github.com/yazgoo/arduino-makefile).


Récupération de la bibliothèque
-----------

J'ai récupéré [la bibliothèque](https://github.com/01010101/Minitel/),
[J'ai modifié le fichier de demo](https://github.com/yazgoo/minitel_twitter/blob/master/demo.cpp)
 pour qu'il builde avec mon Makefile.
On branche le cable, lui même branché à l'arduino au minitel.
On crée un projet pour la demo (c.f. Présentation du Makefile),
et on fait un ``make upload``.
Si ça marche bien, le minitel doit afficher de charmantes démos.


Utilisation de la bibliothèque
-----------

J'ai décidé d'utiliser les méthodes d'affichage de texte pour envoyer
des tweets sur le minitel, il est disponible [ici](https://github.com/yazgoo/minitel_twitter).
Je crée donc un nouveau projet, où je copie Minitel.cpp et Minitel.h.
J'écrit mon fichier principal, minitel.cpp, qui affichera tout texte
reçu via le série USB:

```C++
#include "Arduino.h"
#include <SoftwareSerial.h>
#include "Minitel.cpp"
Minitel* m;
void setup() {
  /* écoute sur les pins 6 et 7 */
  m = new Minitel(6,7);
  /* écoute sur le port série (via USB) */
  Serial.begin(1200);
}
void demoText() {
  m->clearScreen();
  m->mode(TEXT_MODE);
  m->textColor(WHITE);
  m->bgColor(BLACK);
  m->blink();
  m->noBlink();
  int width = 35;
  int i = 1;
  int j = 0;
  char text[2];
  /* pour chaque caractère reçu */
  while(true)
  {
      if(Serial.available())
      {
          text[0] = Serial.read();
          /* si on reçoit un line feed, on change de ligne */
          if(text[0] == '\n')
          {
              j++;
              i = 1;
          }
          else
          {
              text[1] = 0;
              /* affiche la lettre en question */
              m->text(text, i++, 1 + j);
              /* si on dépasse width, on change de ligne */
              if(i > width )
              {
                  j++;
                  i = 1;
              }
          }
      }
  }
}
void loop()
{
    demoText();
}
```

Voici le makefile correspondant:

```
MAIN=minitel
SERIAL_DEVICE=/dev/ttyACM0
DEPENDENCIES=wiring_analog.c WInterrupts.c wiring_digital.c \
    wiring_shift.c wiring_pulse.c wiring.c IPAddress.cpp \
    Stream.cpp Tone.cpp main.cpp CDC.cpp WMath.cpp \
    WString.cpp Print.cpp USBCore.cpp new.cpp HID.cpp \
    HardwareSerial.cpp
LIBRARIES=SoftwareSerial_SoftwareSerial
include /home/yazgoo/Dévelopement/arduino/arduino_makefile/Makefile
```

Ensuite, je crée le fichier python suivant, qui récupérer
n tweets et les écrit sur le port série:

```python
import serial
import time
import json
import sys
import urllib.request as urllib_request
# quel feed lire ?
who = sys.argv[1]
# combien de tweets lire
num = sys.argv[2]
# on récupère les tweets au format json
handle = urllib_request.urlopen("https://api.twitter.com/1/statuses/user_timeline/" + who + ".json?count=" + num + "&include_rts=1")
data = handle.read()
tweets = json.loads(data.decode('utf8'))
# on ouvre le port série à 1200 bauds
ser = serial.Serial('/dev/ttyACM0', 1200)
time.sleep(2)
# pour chaque tweet, on écrit les caractères un par un.
for tweet in tweets:
    text = tweet['text']
    print(text)
    i = 0
    for c in text:
        ser.write(bytes('' + c, 'ISO8859-1'))
        if i % 15 == 0:
            time.sleep(1)
        else:
            time.sleep(0.003)
        i += 1
    ser.write(bytes('\n', 'ISO8859-1'))
    ser.write(bytes('\n', 'ISO8859-1'))
```

Après avoir uploadé sur l'arduino,
on a plus qu'à lancer le code client:
<code>python client.py climagic 3</code>
Ici, on récupère les 3 dernies tweets de climagic.

Conclusions
-----------

Ce code n'est pas du tout léché et a été réalisé rapidement hier.
Comme on s'est contenté d'afficher du texte, on aurait pu transmettre
tout directement en série sans utiliser la bibliothèque Minitel.
C'est ce qu'ont fait d'autres personnes lors de cet atelier,
en transmettant directement leur leur tty via agetty sur le minitel.
Enfin, je vous conseille, si vous êtes sur toulouse, de passer
au THSF.
