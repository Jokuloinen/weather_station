# Sääasema

Toisen opiskeluvuoden tietotekniikan laite- ja tuotesuunnittelun opiskelijoiden lukuvuoden viimeinen ryhmäprojekti.
![kuva sääasemasta](assets/asema.png "sääasema")


## Järjestelmä kaavio

Alhaalla oleva kaavio kuvaa sitä mistä meidän sääasemamme koostuu ja miten nämä osat liittyvät toisiinsa.
![kaavio asemasta](assets/järjestelmäarkitehtuuri.png "arkitehtuurikavio")


## Sääasema

`station` kasniossa on itse sääaseman koodi. Koodi itsessään hoitaa kahta asiaa. Tiedonsiirtoa bluetoothilla ja sensoreiden tulkitsemista.
Osa sensoreista ja kaikki "ICajurit" on eroteltu omiin tiedostoihin koodin lukemisen helpottamiseksi.


## Käyttö liittymä

`GUI` kansion koodi toimii sääaseman käyttöliittymänä. Sillä sääasemaa voidaan käskeä ottamaan mittaus tai lähettämään sen aikaisemmin mittaamat mittaukset.
![gui kuva](assets/gui.png "käyttöliittymä")
