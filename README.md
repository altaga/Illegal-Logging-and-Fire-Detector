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

/// SOLUTION PLACEHOLDER

With the PSoC 62S2 Wi-Fi BT Pioneer Kit, i will obtain an audio signal which, through SensiML model, we can pass through a neural network. That will tell us if the noise of a saw cutting the trees or human voice in the forest.

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

## Hardware Diagram:

<img src="https://i.ibb.co/cDg0DL7/Untitled-Sketch-bb.png">

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

El sensor MQ135 es un dispositivo analogico, por lo tanto tenemos que conectarlo a un pin especifico de lectura analogica.

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

# LoraWAN Module:

In this section we will explain all the details of the data transmission since we send the POsC data  to WiFi-LoRa-32, until it reaches AWS IoT.

## Hardware:

The hardware used for this module was a B-L072Z-LRWAN1 and the X-NUCLEO-IKS01A3 sensor combo.

* B-L072Z-LRWAN1.
  * https://www.st.com/en/evaluation-tools/b-l072z-lrwan1.html
* X-NUCLEO-IKS01A3.
  * https://www.st.com/en/ecosystems/x-nucleo-iks01a3.html

## Sofware:

The board software will be in the Serial2Lora_STM32L0 folder where the Arduino IDE project will be.

<hr>

### **Configure the credentials in the TTN creds.h file**

(details of these credentials in the [TTN](#ttn) section).

    static const char *appEui  = "XXXXXXXXXXXXXXXX";
    static const char *appKey  = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    static const char *devEui  = "XXXXXXXXXXXXXXXX";

<hr>

### **Set the correct frequency of LoraWAN**

US915 for Mexico.

    #define REGION_US915

<hr>

### **Serial to LoraWAN Processing**

    if (Serial1.available()) {
      String temp = Serial1.readString(); // Reading Serial

      // Processing serial read to get classification only

      int checkpoint1 = temp.indexOf(":", 25);
      int checkpoint2 = temp.indexOf(",", 25);
      temp = temp.substring(checkpoint1 + 1, checkpoint2);

      // Append classification in lora message

      my_string = temp + "," + String(int(humidity)) + "," + String(int(temperature)) + "," + String(int(pressure));
      uint8_t payload[my_string.length() + 1];
      my_string.getBytes(payload, my_string.length() + 1);

      // Send to TTN 

      sendResult(payload, my_string.length() + 1);
      delay(1000);
    }

## TTN:

To communicate the device with TTN, we first need to be in range of the TTN gateways.

Check coverage at: https://www.thethingsnetwork.org/map

If you are in coverage, first we have to create a TTN application as the official documentation says:

https://www.thethingsnetwork.org/docs/applications/add/

<img src="./Images/appttn.png">

Now that we have our app we must register our device to obtain the credentials.

https://www.thethingsnetwork.org/docs/devices/registration/

<img src="./Images/device.png">

## TTN to AWS IoT:

My first attempt to communicate TTN with AWSIoT was through the official TTN documentation. However, its integration is not updated as of today 05/04/2021.

https://www.thethingsnetwork.org/docs/applications/aws/

In this case I decided to do my own integration through the following AWS services.

<img src="./Images/aws.png">

<hr>

### **TTN MQTT to AWS API Gateway**:

Since I want this to be a reproducible integration to any system, I decided to make an integration based on the NodeJS container, this container is in the AWS Integration\Container folder.

In a simple way, the container receives all the data that the ttn app receives through the MQTT API and sends everything to its own API as request.

    client.on('message', function (topic, message) {
        console.log(message.toString())
        unirest('POST', data.myAPIurl)
            .headers({
                'Content-Type': 'application/json'
            })
            .send(message.toString())
            .end(function (res) {
                if (res.error) throw new Error(res.error);
                console.log(200);
            });
    })

To deploy this container on AWS use the ECR service to upload the container to AWS and ECS to deploy it. However, you can deploy it on your computer through Docker without any problem.

NOTE: The code will be fully commented on what it is doing.

<hr>

### **AWS API Gateway to AWSLambda**:

Once the message reached AWS API Gateway, we have to perform some action with it, for this we deploy a Lambda code in the API to redirect the complete message to AWS IoT.

<img src="./Images/apigate.png">

<hr>

### **AWSLambda to AWS IoT**:

The program that is executed when the message arrives from the container is the following. Also the code is in the AWS Integration \ Lambda folder.

    import json
    import boto3

    client = boto3.client("iot-data")

    def lambda_handler(event, context):
        response = client.publish(
                topic="ttn/echo",
                qos=1,
                payload=json.dumps(event["body"]))
        return {
            'statusCode': 200,
            'body': json.dumps('TTN Correct!')
        }

All the messages that we send from our device, by TTN to AWSIoT, will reach the topic ttn/echo.

<hr>

### Backend DEMO:

Here is a demonstration of the entire backend running at the same time.

Video: Click on the image
[![Capture](./Images/awsint.png)](https://youtu.be/p_ZSyW9KK2k)

# Power Consumption:

For the project it is very important to see the energy consumption of our device, so I decided with the Nordic Power Profiler Kit II, to do an analysis of the mAh consumed by the device.

The complete initialization of the entire device is approximately 40 seconds with an average consumption of 45mA and peaks of 132mA.

<img src="./Images/138.png">

However, what interests us is the long-term power consumption, so analyzing the consumption after the initialization, we find a stable zone of 48.7 mAh.

<img src="./Images/48.png">

So the LoraWAN module only adds 10mAh to the consumption of the device.

Video: Click on the image
[![PPKII](./Images/none.png)](https://youtu.be/6QJh2mOm6js)

# WebPage Deploy:

The deployment of the web page was done using ReactJS, OpenLayers (Maps) and AWS-SDK for javascript.

Desktop:

<img src="./Images/desk.png" height="300px">

Mobile:

<img src="./Images/mobile.png" height="300px">

https://illegal-logging-detector.s3.amazonaws.com/index.html

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

The web page receives the sensor data through AWSIoT as a web socket, so it is important to define within the page, which is the topic that we are going to receive, in this case "ttn / echo" as we could see in the video of [Backend Video](# backend-demo).

In the following file, put the name of the topic to which you will be subscribed.

Webapp/src/App.js

    <IotReciever sub_topics={["ttn/echo"]} callback={this.callBackIoT} />

### **Decode IN Data**:

Because the data we receive from TTN is with a base64 encoding, we need to decode it with the following code in the webapp.

    let payload = atob(temp["payload_raw"]).replace(' ', '').split(",")

This performs a conversion like the one we see in the image.

<img src="./Images/decode.png">

The array is made up of 4 pieces of information:

1. Result of the neural network.
2. Realtive humidity of the environment.
3. Temperature in degrees Celsius (converted to Fahrenheit on the page)
4. Atmospheric pressure in mmHg.

# Final Product:

Case Closed:

<img src="./Images/dev1.jpg">

Case open:

<img src="./Images/dev2.jpg">

Platform UI:

<img src="./Images/desk.png" height="300px">
<img src="./Images/mobile.png" height="300px">

https://illegal-logging-detector.s3.amazonaws.com/index.html

# EPIC DEMO:

Video: Click on the image
[![demo](./Images/logo.png)](https://youtu.be/eDP3U5mweT8)

Sorry, github does not allow embed videos.