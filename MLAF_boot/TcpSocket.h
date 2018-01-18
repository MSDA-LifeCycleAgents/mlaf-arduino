#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "AclMessage.h"
#include "MessageParser.h"

class TcpSocket{
  private:  
    int default_port = 1234;
    WiFiServer *server;
    WiFiClient client;
    IPAddress ip;
    int port;
  
  public:
    String getIpAddressAsString(){
      return String(ip[0]) + String(".") + String(ip[1]) + String(".") + String(ip[2]) + String(".") + String(ip[3]);
    }

    TcpSocket(){}

    void init(String wifi_ssid, String wifi_pass, int _port){
      port = _port;
      server = new WiFiServer(port);
      
      if(WiFi.status() == WL_NO_SHIELD){
        Serial.println("No WiFi shield present. Connection will not be established.");
        while(true);
      }
      
      WiFi.mode(WIFI_STA);

      char ssid_arr[50];
      wifi_ssid.toCharArray(ssid_arr, 50);

      char pass_arr[50];
      wifi_pass.toCharArray(pass_arr, 50);
      
      WiFi.begin(ssid_arr, pass_arr);
      
      while (WiFi.status() != WL_CONNECTED) { 
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected.");
      
      ip = WiFi.localIP();
      server->begin();
    }

    AclMessage* listen(){
      String request;
      AclMessage* message = NULL;
      client = server->available();
      if(client){
        boolean currentLineIsBlank = true;
        while (client.connected()) {
          if (client.available()) {
            char c = client.read();
            request += c;
            if (c == '\n' && currentLineIsBlank) {
              MessageParser parser;
              message = parser.fromXml(request);
              Serial.println(request);
              request = "";
              break;
            }
            if (c == '\n') {
              currentLineIsBlank = true;
            } else if (c != '\r') {
              currentLineIsBlank = false;
            } 
          }
        }
      }
      return message;
    }

    int send(AclMessage* message){
      if(client.connect(message->receiver->getAddress(), message->receiver->getPort())){   
        Serial.println("Starting message parser"); 
        MessageParser parser;
        String parsedMessage = parser.toXml(message);
        Serial.println("Socket sending message: " + parsedMessage);
        int result = client.println(parsedMessage);
        if(result > 0){
          AclMessage::destroy(message);
        }
        return result;
      }
      Serial.println("Could not connect to: " + message->receiver->getAddress() + ":" + String(message->receiver->getPort()));
      return -1;
    }

    void advertise(String name, String description, String timestamp){
      String ipText = getIpAddressAsString();
      String locator = "tcp://" + ipText + String(":") + String(port);
      
      String agentName = name + String("@") + ipText + String(":") + String(port) + String("/JADE");
      agentName.replace(".", "?");

      char agentNameArr[50];
      agentName.toCharArray(agentNameArr, 50);
      
      MDNS.begin(agentNameArr);
      MDNS.addService("fipa_service_directory", "udp", port);
      MDNS.addServiceTxt("fipa_service_directory", "udp", "LOCATOR", locator);
      MDNS.addServiceTxt("fipa_service_directory", "udp", "DESCRIPTION", description);
      MDNS.addServiceTxt("fipa_service_directory", "udp", "TIMESTAMP", timestamp);
      MDNS.addServiceTxt("fipa_service_directory", "udp", "TYPE", "mts_client");
    }
  
};

