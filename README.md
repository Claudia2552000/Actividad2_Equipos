# Actividad2_Equipos
Entrega actividad 2 de equipos e instrumentación electrónica. Grupo: Claudia Betancor Soca, Leticia Esther Santana Robaina y María Almudena Jiménez Suárez

## Proyecto: Sistema de Monitoreo Ambiental con Arduino

Este proyecto implementa un sistema de monitoreo de condiciones ambientales usando Arduino UNO. Permite visualizar temperatura, humedad, nivel de luz, velocidad del viento y calidad del aire, además de accionar LEDs indicadores y mostrar datos en una pantalla LCD I2C.

**Características del Sistema**

*Lectura de sensores:* 
- Temperatura y humedad (DHT22)
 - Luz ambiental (LDR)
 - Calidad del aire (potenciómetro)
- Velocidad del viento (potenciómetro)

*Visualización de datos en pantalla LCD I2C (16x2)*

*Control automático de LEDs:*

- LED azul (calefacción)
- LED rojo (ventilador)
- LED blanco (luz artificial)
- LED RGB (indicador de calidad del aire)

*Activación de visualización de datos mediante pulsador*

**Librerías utilizadas**

         #include <Wire.h>                // Comunicación I2C
         #include <LiquidCrystal_I2C.h>   // Manejo de pantalla LCD
         #include <DHT.h>                 // Sensor de temperatura y humedad
         #include <math.h>                // Cálculos matemáticos (lux)

**Asignación de Pines**

| Componente           | Pin             | Descripción                    |
|----------------------|-----------------|--------------------------------|
| Sensor DHT22         | 2               | Lectura de temperatura y humedad |
| Sensor de luz (LDR)  | A0              | Medición de luz ambiental      |
| Potenciómetro viento | A1              | Simulación de velocidad del viento |
| Potenciómetro aire   | A2              | Simulación de calidad del aire |
| LED rojo             | 9               | Indicador de calor             |
| LED azul             | 8               | Indicador de frío              |
| LED blanco           | 10              | Luz artificial por baja luz    |
| LED RGB (aire)       | 5 (R), 6 (G), 7 (B) | Indicador de calidad del aire  |
| Pulsador             | 4               | Muestra datos en la pantalla LCD |
| LCD I2C              | SDA/SCL (A4/A5) | Pantalla para visualización    |


**Lógica del Sistema**

*Lectura de sensores*

- DHT22: Mide temperatura y humedad.

- LDR: Determina el nivel de luz y calcula lux con una fórmula basada en resistencia.

- Potenciómetros: Simulan velocidad del viento y calidad del aire.

*Clasificación de la calidad del aire*

- Mala: < 33

- Regular: 33-65

- Buena: > 66

*Visualizada con LED RGB:*

- Buena: verde

- Regular: naranja

- Mala: roja

*Control automático de LEDs*

- Si temperatura < 18 °C: LED azul (frío) encendido.

- Si temperatura > 28 °C: LED rojo (calor) encendido.

- Si luz < 300 lux: LED blanco (luz artificial) encendido.

*Visualización en LCD*

- Activada pulsando el botón.

- Muestra:

	1. Temperatura y humedad

	2. Luz (lux) y viento (km/h)

	3. Calidad del aire

**Ciclo de ejecución (loop())**

- Lee todos los sensores.

- Clasifica condiciones y acciona LEDs.

- Si se presiona el botón, muestra los datos en LCD durante 3 secciones.

- Apaga la pantalla tras mostrar la información.

