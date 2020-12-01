#include <SPI.h>
#include <Ethernet.h>
String readString;

int pino_rele1 = 3;
int pino_rele2 = 4;
boolean ligado = true;
boolean ligado_2 = true;

// Informacoes de endereco IP, gateway, mascara de rede
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = {10,0,0,125}; // defina aqui seu ip de acordo com a faixa definida no seu roteador.

EthernetServer server(80);

void setup()
{
  Serial.begin(9600);
  pinMode(pino_rele1, OUTPUT);
  pinMode(pino_rele2, OUTPUT);

  //Inicializa Ethernet Shield
  Ethernet.begin(mac, ip);
  server.begin();

  Serial.println("INOVA 2020");

  // Desliga os dois reles
  digitalWrite(pino_rele1, HIGH);
  digitalWrite(pino_rele2, HIGH);
} // Setup

void loop()
{
  // INICIALIZA O CLIENTE ETHERNET
  EthernetClient client = server.available();

  // SE A PLACA ETHERNET ESTIVER DISPONIVEL
  if (client) {

    // ENQUANTO ESTIVER CONECTADO
    while (client.connected())
    {

      // SE DISPONIVEL
      if (client.available())
      {
        char c = client.read();
        if (readString.length() < 100) {
          readString += c;
        }
        if (c == 'n')
        {
          //Controle do rele1
          Serial.println(readString);
          //Liga o Rele 1
          if (readString.indexOf("?ligar") > 0)
          {
            digitalWrite(pino_rele1, LOW);
            Serial.println("Rele 1 Ligado");
            ligado = false;
          }
          else
          {
            //Desliga o Rele 1
            if (readString.indexOf("?desligar") > 0)
            {
              digitalWrite(pino_rele1, HIGH);
              Serial.println("Rele 1 Desligado");
              ligado = true;
            }
          }
 
          //Controle do rele2
          Serial.println(readString);
          //Liga o Rele 2
          if (readString.indexOf("?2_ligar") > 0)
          {
            digitalWrite(pino_rele2, LOW);
            Serial.println("Rele 2 Ligado");
            ligado_2 = false;
          }
          else
          {
            //Desliga o Rele 2
            if (readString.indexOf("?2_desligar") > 0)
            {
              digitalWrite(pino_rele2, HIGH);
              Serial.println("Rele 2 Desligado");
              ligado_2 = true;
            }
          }
          readString = "";

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<html>");
          client.println("<head>");
          client.println("<title>INOVA 2020</title>");
          client.println("<meta http-equiv='Content-Type' content='text/html; charset=ISO-8859-1'>");
          client.println("<meta name='viewport' content='width=720, initial-scale=0.5' />");
          client.println("<link rel='stylesheet' type='text/css' href='http://apresentacoes.closetothemetal.club/assets/css/inova2020.css' />");
          client.println("<script type='text/javascript' src='http://apresentacoes.closetothemetal.club/assets/scripts/inova2020.js'></script>");
          client.println("</head>");
          client.println("<body>");
          client.println("<div id='wrapper'><img alt='INOVA 2020' src='http://apresentacoes.closetothemetal.club/assets/images/inova2020/logo-inova.png'/><br/>");
          client.println("<div id='div1'>Rel&eacute; 1</div>");
          client.println("<div id='div2'>Rel&eacute; 2</div>");
          client.print("<div id='rele'></div><div id='estado' style='visibility: hidden;'>");
          client.print(ligado);
          client.println("</div>");
          client.println("<div id='botao'></div>");
          client.println("<div id='botao_2'></div>");
          client.print("<div id='rele_2'></div><div id='estado_2' style='visibility: hidden;'>");
          client.print(ligado_2);
          client.println("</div>");
          client.println("</div>");
          client.println("<script>AlteraRele1()</script>");
          client.println("<script>AlteraRele2()</script>");
          client.println("</div>");
          client.println("</body>");
          client.println("</head>");
 
          delay(1);
          client.stop();
        }
      }
    }
  }
}
