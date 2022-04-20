# Driver LIS2DW12

## Introducción

Se trata de un driver genérico, con portabilidad de momento para la plataforma STM32F429XX.  
Aunque el acelerómetro LIS2DW12 soporta tanto SPI como I2C, este driver está basado únicamente en I2C, ya que es de la forma que más se lo comercializa en módulos (como por ejemplo el I2C Gravity LIS2DW12 de DFROBOT).  
El driver provee una implementación de la hoja de datos del acelerómetro de 3 ejes LIS2DW12 de ST mediante el acceso a todo el mapa de registros mediante I2C.  
La hoja de datos puede encontrarse en *./doc/LIS2DW12.pdf*.  
Además, el driver proporciona ciertas APIs de mayor abstracción que ejecutan ciertas configuraciones de interes como se detalla en la nota de aplicación de ST.  
La nota de aplicación puede encontrarse en *./doc/LIS2DW12: always-on 3D accelerometer - Application note.pdf*

## Árbol de directorios

Está compuesto por tres directorios de la siguiente manera:  

```bash
└── LIS2DW12
    ├── doc
    │   ├── LIS2DW12.pdf
    │   └── LIS2DW12: always-on 3D accelerometer - Application note.pdf
    ├── inc
    │   ├── lis2dw12.h
    │   ├── lis2dw12_cfg.h
    │   └── lis2dw12_stm32f429xx_port.h
    ├── src
    │   ├── lis2dw12.c
    │   └── lis2dw12_stm32f429xx_port.c
    ├── Doxyfile.txt
    └── readme.md
```
El directorio *doc* contiene la documentación sobre la que está basado el diseño del driver y también contiene la documentación generada por *Doxygen* de la API del driver.  
El directorio *inc* incluye los archivos de cabecera del driver:  
+ **lis2dw12.h** contiene la API pública del driver, i.e. las funciones a llamar en el programa principal para utilizar el driver.
+ **lis2dw12_cfg.h** contiene los parámetros de configuración del driver.
+ **lis2dw12_stm32f429xx_port.h** contiene los prototipos del *wrapper* de la API de I2C de la HAL de ST.  
El directorio *src* incluye los archivos fuentes del driver:  
+ **lis2dw12.c** contiene la implementación de la API pública del driver y para ello se basa en una API privada definida en el mismo archivo.
+ **lis2dw12_stm32f429xx_port.c** contiene el *wrapper* de la API de I2C de la HAL de ST.  

## Diseño

El diseño está basado en su comunicación I2C estándar de 100KHz y en su mapa de registros.  
Teniendo en cuenta que el diseño inicial del driver está basado en módulos comerciales sencillos como el I2C Gravity LIS2DW12, el tipo de driver que más se ajusta es un *polling driver* o driver de encuesta.  
La razón es sencilla, la mayoría de los módulos comerciales no dan acceso a los pines de interrupción (en este caso INT1 e INT2), por lo que la configuración como un *interrupt driver* está limitada por hardware. Igualmente, la API privada del archivo *lis2dw12.c* permite facilmente extender la API pública para que permita al driver configurarse como un *interrupt driver*, y no sólo un *polling driver*.  
Finalmente, queda descartado la configuración del driver como un *DMA driver*, porque resultaría en una sobredimensión de la solución. El LIS2DW12 soporta los modos *fast* (400Khz) y *standard* (100Khz) de I2C, que comparados a la velocidad de reloj del microcontrolador que hasta ahora soporta el driver (alrededor de 200MHz para STM32F429XX) deja en evidencia que el I2C es en realidad lento. Con esto en mente, la posibilidad de perder datos estará más a cargo a un *crosstalk* (interferencias por el diseño de pistas en hardware) que por el procesamiento del microcontrolador.  
De cualquier manera, se podría agregar o modificar el archivo *port* para que soporte DMA para una aplicación especifica en la que se quiera asegurar la recepción de los datos o que el sistema esté muy cargado y resulte eficiente liberar al microcontrolador de leer registros (aunque en el LIS2DW12 nunca se leeran mas de 16 bits por transacción I2C).
