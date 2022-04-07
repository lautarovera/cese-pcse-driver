# CESE PCSE - Implementación de Driver Genérico
El presente es un repositorio que contiene la implementación de un driver genérico. 
El mismo tiene modalidad de examen, siendo necesario cumplir los siguientes requisitos para la aprobación del curso Protocolos de Comunicaciones en Sistemas Embebidos (CESE).
## Requerimientos
Los requerimientos aquí listados suponen una base mínima que debe cumplirse, y no son bajo
ningún concepto restricciones. Si el estudiante desea sobrepasar estos requerimientos mínimos
con funcionalidades adicionales, tiene la libertad de hacerlo. Bajo ningún concepto se tomará por
aprobado el estudiante que no cumpliere con los requerimientos mínimos.
1. Implementación de un pequeño prototipo funcional demostrativo de comunicación entre la
plataforma de hardware a utilizar (placa núcleo o ESP32) y un periférico, con alguno de
los protocolos vistos en las distintas clases. Se insta a que este prototipo demostrativo
sea del tipo modular, de manera que no sea necesario diseñar una PCB específica para
el mismo, sino que utilice módulos disponibles en el mercado.
2. Implementación de un driver para el prototipo mencionado en el requerimiento número 1,
el cual siga las bases conceptuales y estructura de datos vistas en la clase device drivers.
En tal sentido, el driver implementado deberá poseer:
a. Una estructura de archivos como la propuesta en clase, abstrayendo por medio de
las funciones definidas en port.c el hardware específico. Estas funciones deben
ser lo más cercano al hardware, por lo que serán de “bajo nivel”, comparables a la
capa número 2 del modelo OSI (data link). Esto es conocido como specific-port.
b. Las funciones del driver genérico deben estar en archivos fuente separados de la
parte específica para el hardware.
3. El tipo de driver utilizado puede ser del tipo que el estudiante desee (Polled driver,
orientado a interrupciones u orientado a acceso directo a memoria).
4. El driver deberá ser subido a un repositorio público y puesto a disposición de los
docentes.
5. El repositorio debe estar organizado en los directorios /inc y /src y un archivo README
que explique la intención y funcionalidades del driver.
6. Opcional: Generación de documentación con doxygen.
7. Se deberá hacer una presentación del estilo exposición magistral, en donde se evaluarán
distintos puntos, a saber:
    - Exposición clara de conceptos.
    - Capacidad de síntesis al presentar la información.
    - Manejo del tiempo, el cual estará limitado a un máximo de 10 minutos.
