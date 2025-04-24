# ESP8266 P1 Dongle – Energieverbruik uitlezen via MQTT

Dit project toont hoe je een dongle bouwt met een **ESP8266** om een digitale gas- en elektriciteitsmeter uit te lezen via de **P1-poort**. De verbruiksdata wordt via **MQTT** doorgestuurd naar een broker, zoals **HiveMQ**.

De implementatie bevat:
- Tussenstappen met **Domoticz** voor validatie tijdens de ontwikkeling
- Geen gebruik van Domoticz in de uiteindelijke opstelling
- Een werkende dongle als eindresultaat

🔧 Gebaseerd op de YouTube-video van Maarten Raeymaekers:  
[https://www.youtube.com/watch?v=bg7HV00wcss](https://www.youtube.com/watch?v=bg7HV00wcss)

⚠️ Let op: dit project behandelt **niet** hoe je een MQTT-broker opzet. Gebruik bijvoorbeeld HiveMQ of een eigen bestaande broker.

🎯 Doelpubliek: hobbyisten, makers, en Domoticz/MQTT-gebruikers die hun energieverbruik willen automatiseren of visualiseren.
