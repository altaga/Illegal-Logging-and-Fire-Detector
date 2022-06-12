# Illegal Logging and Fire Detector

<img src="https://i.ibb.co/4RrNrgx/logo.png">

# Introduction:

Avoid illegal logging in protected areas.

We can find that just in Mexico (my home country) [1] 70 percent of the wood consumed is of illegal origin according to a study carried out by one of the most prestigious universities UNAM (QS ranking # 100).

https://translate.google.com/translate?sl=es&tl=en&u=https://www.dgcs.unam.mx/boletin/bdboletin/2018_173.html

<img src="https://i.ibb.co/YXRmdkN/image.png" >

I’ll  create a system that is capable of recognizing, through Machine Learning the sounds generated by falling trees, chainsaws and human voices in protected areas, thus warning that illegal logging may be occurring.

I especially want a system that can help protect forests and the species that inhabit them.

<img src="https://i.ibb.co/5xvc3Rb/Fores-1.jpg" height="49%" ><img src="https://i.ibb.co/nm2qk9D/Fores-2.jpg" height="49%" >

Most solutions are based on raising awareness, but looking at more dedicated solutions we can find:

TreeTAG is an emerging smartphone-based supply chain traceability system developed by Earth Observation Systems that tracks the location of logs transported from the forest to the mill.

Disadvantages: Very complex system that requires authorized personnel to be manipulated.

Stardust is a dust-like material that can be sprayed onto wood and detected only with a hand-held device. 

Disadvantages: You need to tag manually every tree which is labor intensive and expensive..

# Solution:

The system, will be easily reproducible, energy efficient and powerful thanks to the ML algorithms that will be implemented combined with the cloud services that we will use for deployment.

<img src="https://i.ibb.co/TRcMLMW/ezgif-com-gif-maker-14.gif" width="100%">

With the PSoC 62S2 Wi-Fi BT Pioneer Kit, i will obtain an audio signal which, through SensiML model, we can pass through a neural network. That will tell us if the noise of a saw cutting the trees or human voice in the forest, and MQ135 for fire smoke.

Displaying the information of the events detected in a simple webapp, together with a map which will indicate the position of the event.

## Features:

* Low-power battery consumption (PSoC and LoraWAN).
* High accuracy (thanks to SensiML).
* Easy production at large scale, due to its simplicity.

# Hardware, Software and Services:

## Hardware:

* PSoC™ 62S2 Wi-Fi BT Pioneer Kit. 1x.
  * https://www.infineon.com/cms/en/product/evaluation-boards/cy8ckit-062s2-43012/
* CY8CKIT-028-SENSE. 1x.
  * https://www.infineon.com/cms/en/product/evaluation-boards/cy8ckit-028-sense/
* MQ135 Air Quality Sensor. 1x.
  * https://www.amazon.com/Ximimark-Quality-Hazardous-Detection-Arduino/dp/B07L73VTTY/
* WiFi LoRa 32 (V2.1). 1x.
  * https://heltec.org/project/wifi-lora-32/
* LiPo Battery. 1x.
  * https://www.amazon.com/1000mAh-battery-Rechargeable-Lithium-Connector/dp/B07BTWK13N

## Software:

* ModusToolbox™.
  * https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software/
* SensiML.
  * https://sensiml.com/
* Data capture lab.
  * https://sensiml.com/products/data-capture-lab/
* Google Colab.
  * https://colab.research.google.com/
* ReactJS.
  * https://reactjs.org/
* OpenLayers Maps.
  * https://openlayers.org/
* Arduino IDE.
  * https://www.arduino.cc/en/software/

## Services:

* AWS IoT.
  * https://aws.amazon.com/iot/
* AWS IAM.
  * https://aws.amazon.com/iam/
* AWS Cognito.
  * https://aws.amazon.com/cognito/
* AWS S3.
  * https://aws.amazon.com/s3/

# Connection Diagram:

## Backend Diagram:

<img src="https://i.ibb.co/wMhs3Xh/Schene-drawio.png">

- Nuestro device se conecta a la red de LoraWAN mediante el WiFi LoRa 32.
- Ya en la red de Helium el dato se procesa en la Helium Console, conectandose a AWS IoT Gateway.
- En nuestra AWS IoT Integration recibimos en dato mandado por el device a un Topic.
- La pagina web esta suscrita al topic donde estamos mandando los datos del sensor, ya en la pagina desplegamos el resultado.

## Hardware Diagram:

<img src="https://i.ibb.co/cDg0DL7/Untitled-Sketch-bb.png">

- El sensor MQ135 esta conectado a un pin analogico A7 en la PSoC™ 62S2.
- La WiFi LoRa 32 esta conectada a los pines P9_6 y P9_7 para recibir la informacion de que sensor a la board.

NOTA: Los detalles de conexion de ambos diagrama estan explicados mas abajo.

# Capturing Data:

In this section I will explain how to get audio data from the PSoC to the Data capture lab.

## Setting up the PSoC:

First we have to configure the PSoC in data capture mode, por fortuna el proyecto ejemplo dentro del Modus Toolbox ya viene configurado en modo captura de datos en audio, asi que solo tendras que flashearlo en el device para empezar a capturar datos.

<img src="https://i.ibb.co/7jtdjcT/image.png">

Una vez flasheado el dispositivo tendremos que abrir un nuevo proyecto en SensiML con la siguiente configuracion para la captura de datos.

<img src="https://i.ibb.co/ssPx0Bt/image.png">

Si todo funciona correctamente podremos empezar a recibir datos y registrarlos dentro de nuestro proyecto.

Video: Click on the image
[![Capture](https://i.ibb.co/4RrNrgx/logo.png)](https://youtu.be/1NnxkoxOQmY)

## Capturing Data:

In the case of my project, the easiest thing was to record the sound of several chainsaws, people and neutral audio data in order to properly train the model.

NOTE: The captured audios will be in the [SensiML_Data_Capture_Lab](./SensiML_Data_Capture_Lab/).

Video: Click on the image
[![Capture](https://i.ibb.co/4RrNrgx/logo.png)](https://youtu.be/qyJq7049lT8)

## Labeling Data:

In this case, I did the labeling of the following categories for my model.

<img src="https://i.ibb.co/ZMLTgZs/image.png">

The system is capable of detecting the sound of mechanical saws, humanvoice and neutral silence, in order to avoid false alarms of the system.

# SensiML:

These were the specifications for the data in SensiML.

<img src="https://i.ibb.co/YdJmQ91/image.png">

These were the specs of the model's training.

<img src="https://i.ibb.co/TkT5sCx/image.png">

Here the results of the precision of the model against the data used.

<img src="https://i.ibb.co/BHDTwR0/image.png"> 

And finally the specifications of the [Knowledge Pack](Knowledge_Pack.zip) that is in the repository.

<img src="https://i.ibb.co/09NTs9z/image.png">

# Testing Model on PSoC:

In this case you can see in the video how the model works correctly for the detection of human voice and detection of a Chainsaw. We are doing this test by seeing the PSoC serial output.

NOTE: the serial output is at 1000000 baudrate.

<img src="https://i.ibb.co/8myQqLQ/image.png">

Video: Click on the image
[![Capture](https://i.ibb.co/4RrNrgx/logo.png)](https://youtu.be/zllu2-R3VS4)

# PSoC - WiFi-LoRa-32, MQ135 and CY8CKIT-028-SENSE:

## WiFi-LoRa-32:
Nuestro dispositivo WiFi-LoRa-32, ademas de mandar los datos recibidos del PSoC a la red de Helium, provee de energia a todo nuestro dispositivo ya que tiene un modulo de bateria LiPo y un regulados 3.3v.

| PSoC PIN   | WiFi-LoRa-32 PIN |
|------------|------------------|
| 3.3 V      | 3.3 V            |
| GND        | GND              |
| A14 (P9_6) | 21               |
| A15 (P9_7) | 13               |

## MQ135:

El sensor MQ135 es un dispositivo analogico que detecta gases extraños en el ambiente, como lo puede ser humo, alchol, etc... (algunas pruebas se haran con alchol y no con humo por el riesgo de hacer fuego en interiores)

Por lo tanto tenemos que conectarlo a un pin especifico de lectura analogica.

| PSoC PIN   | MQ135 PIN |
|------------|-----------|
| 3.3 V      | 3.3 V     |
| GND        | GND       |
| A7 (P10_7) | AO        |

## CY8CKIT-028-SENSE:

Este dispositivo ya viene totalmente diseñado para utlizarse con la PSoC, sin embargo el poder desplegar imaganes en la pantalla puede ser complicado. En el programa ejemplo de Modus Toolbox podemos encontrar un ejemplo de el logo de Cypress para la pantalla.

<img src="https://i.ibb.co/GQps0yn/image.png">

Como creemos que diseñar todo esto solo con X y _ puedes ser muy engorroso y cansado, preferimos crear un script de python que nos permita convertir una imagen de 128x40 en un arreglo de datos que puedan desplegarse en pantalla.

[Colab Notebook](./Image2PSoC.ipynb)

Como podemos ver en la imagen solo tenemos que subir nuestra imagen y el programa la convertira en un arreglo que se pueda desplegar.

<img src="https://i.ibb.co/fGrqccb/image.png">

Solo tenemos que copiar y pegar el string que nos da el programa y listo.

<img src="https://i.ibb.co/kxbQfHd/image.png">

Asi es como se vera una vez desplegada en la pantalla.

<img src="https://i.ibb.co/THZ3PNx/20220604-194736.jpg">

## All Together:

Una vez ya con todos los dispositivos conectados tenemos el siguiente circuito.

<img src="https://i.ibb.co/rF6V8hb/20220604-194509.jpg" >

# Data Transmission:

In this section we will explain all the details of the data transmission since we send the POsC data to AWS IoT.

## PSoC to Helium Network:

### Hardware:

The hardware used for this module was a WiFi-LoRa-32.

* WiFi LoRa 32 (V2.1). 1x.
  * https://heltec.org/project/wifi-lora-32/
* LiPo Battery. 1x.
  * https://www.amazon.com/1000mAh-battery-Rechargeable-Lithium-Connector/dp/B07BTWK13N

### Sofware:

The board software will be in the [ESP32_Helium_OTAA](./ESP32_Helium_OTAA/) folder where the Arduino IDE project will be.

<hr>

### **Helium Console new Device**:

Tenemos que crear un nuevo device en nuestra consola de Helium, este puede tardar hasta 20 min en empezar a mandar datos a la consola, porfavor no te desesperes.

<img src="https://i.ibb.co/HGkVkts/image.png">

<hr>

### **Configure the credentials in the Helium creds.h file**

Dentro de la carpeta de ESP32_Helium_OTAA tendras que configurar las credenciales que obtuvimos en el paso anterior.

    uint8_t DevEui[] = { 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX };
    uint8_t AppEui[] = { 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX }; 
    uint8_t AppKey[] = { 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX };

<hr>

### **Set the correct frequency of LoraWAN**

US915 for Mexico.

<img src="https://i.ibb.co/0Dnbw6R/image.png">

<hr>

### **Serial to LoraWAN Processing**

Cada vez que nuestro device va a mandar datos a la red de Helium este hace una lectura de los pines 21 y 13 para ver el valor que esta leyendo la PSoC, debido a que estos valores se mandan cada 10 segundos a la red y cada uno de los mensajes tiene un costo en data credits, por lo tanto solo estamos mandando un caracter que representa la lectura.

| PSoC Detection | Character |
|----------------|-----------|
| Nothing        | '0'       |
| Humans         | '1'       |
| Logging        | '2'       |
| Fire           | '3'       |

<hr>

    case DEVICE_STATE_SEND:
    {
      LoRaWAN.displaySending();
      String test;
      bool human = digitalRead(HUMAN_PIN);
      bool saw = digitalRead(CHAINSAW_PIN);
      if(saw && human){
        test = "3";
      }
      else if(human){
        test = "1";
      }
      else if(saw){
        test = "2";
      }
      else{
        test = "0";
      }
      Serial.print("Human: ");
      Serial.println(human);
      Serial.print("Saw: ");
      Serial.println(saw);
      sendStringData(test);
      deviceState = DEVICE_STATE_CYCLE;
      break;
    }

### Helium Network Coverage:

Debido a que la red de helium es dia a dia mas grande, podemos tener cierta certeza que nuestros devices tendran coverage en casi cualquier lugar.

https://explorer.helium.com/

<img src="https://i.ibb.co/NrGfyT1/image.png">

Ahora como muestra aqui un ejemplo de como nuestro device esta mandando datos a la red de Helium.

Video: Click on the image
<img src="https://i.ibb.co/MsMXg3H/image.png">

## Helium Network to AWS IoT:

Para comunicar la plataforma de Helium Network con AWs IoT, decidi utiliar la propia integracion que Helium ya tiene desarrollada para evitarme contratiempos tratando de implementar una propia.

<img src="https://i.ibb.co/nB4Q6dv/image.png">

Esta integracion se basa en crear una credencial de IAM de AWS para que Helium Console pueda realizar las publicaciones correspondientes en AWS IoT.

<img src="https://i.ibb.co/bBPCWFZ/image.png">

### Backend DEMO:

Here is a demonstration of the entire backend running at the same time.

Video: Click on the image
[![Capture](https://i.ibb.co/4RrNrgx/logo.png)](https://youtu.be/0V7A9kYDo9o)

# WebPage Deploy:

The deployment of the web page was done using ReactJS, OpenLayers (Maps) and AWS-SDK for javascript.

https://illegal-logging-and-fire-detector.s3.amazonaws.com/index.html

<img src="https://i.ibb.co/sHN7Szb/image.png">

## AWS Cognito:

For security, to safely use and consume AWS services, **identity pool** credentials were implemented with the Cognito service.

The access keys for AWSIoT and Cognito must be placed in the following file.

Webapp/src/components/aws-configuration.js

    var awsConfiguration = {
      poolId: "us-east-1:XXXXXXXXXXXXXXX", // 'YourCognitoIdentityPoolId'
      host:"XXXXXXXXXXXXXX-ats.iot.us-east-1.amazonaws.com", // 'YourAwsIoTEndpoint', e.g. 'prefix.iot.us-east-1.amazonaws.com'
      region: "us-east-1" // 'YourAwsRegion', e.g. 'us-east-1'
    };
    module.exports = awsConfiguration;

## AWS IoT WebSocket:

The web page receives the sensor data through AWSIoT as a web socket, so it is important to define within the page, which is the topic that we are going to receive, in this case "/helium/devices" as we could see in the video of [Backend Video](# backend-demo).

In the following file, put the name of the topic to which you will be subscribed.

[WebApp/src/App.js](./WebApp/src/App.js)

    <IotReciever sub_topics={["/helium/devices"]} callback={this.callBackIoT} />

### **Decode IN Data**:

Because the data we receive from Helium Payload is base64 encoding, we need to decode it with the following code in the webapp.

    let temp = {
      name: JSON.parse(data[1]).name,
      payload: atob(JSON.parse(data[1]).payload),
    }

This performs a conversion like the one we see in the image.

<img src="https://i.ibb.co/rwsMbHs/image.png">

Dependiendo del resultado del Payload podremos saber que situacion esta detectando el device.

| Detection Kind | Payload   |
|----------------|-----------|
| Nothing        | '0'       |
| Humans         | '1'       |
| Logging        | '2'       |
| Fire           | '3'       |

A su vez podremos saber el tipo de situacion debido al simbolo mostrado en el mapa.

<img src="https://i.ibb.co/r6Nd5Dg/New-Project-10.png">

# Mini DEMO:

Video: Click on the image
[![Capture](https://i.ibb.co/4RrNrgx/logo.png)](https://youtu.be/SdWnbve8zJ8)

# Final Product:

Case Closed:

<img src="https://i.ibb.co/gtWC3Jb/20220611-142331.jpg">

Case open:

<img src="https://i.ibb.co/CMJW55T/20220611-142714.jpg">

Platform UI:

<img src="https://i.ibb.co/r6Nd5Dg/New-Project-10.png">

Page URL:

https://illegal-logging-and-fire-detector.s3.amazonaws.com/index.html

# EPIC DEMO:

Video: Click on the image
[![demo](https://i.ibb.co/4RrNrgx/logo.png)](https://youtu.be/W7tBl_o99-Q)

Sorry, github does not allow embed videos.