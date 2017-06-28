Car Manager es un proyecto donde se busca automatizar todos los componentes de un determinado vehículo.
Desvinculando al usuario de tareas que interrumpen la atención en el camino y favorecen al usuario brindándole un mayor confort al volante

Para este proyecto, se utilizó un Arduino UNO para toda la parte más vinculada al hardware.
- Con este, se automatizó los faros del vehículo, de forma que estos se encienden automáticamente al momento que se detecta baja intensidad lumínica en la ruta
- Al momento de lluvia, esta es detectada y se dispara automáticamente el limpiaparabrisas. 
-Adicionalmente, cuando el usuario desea estacionar y pone la marcha atrás. Si está lloviendo, automáticamente se encienden los limpiaparabrisas traseros, que permiten la visibilidad de la luneta trasera.
-También, cuando se pone la marcha atrás, se enciende el sensor de proximidad ubicado en la parte posterior del auto. Mediante a un display, se representa en metros, la distancia que se tiene al objeto más próximo

El sistema mencionado anteriormente, se puede vincular de forma cómoda y eficaz a un Smartphone Android, por medio de BlueTooth.
- Se establece la vinculación con el Arduino, por medio de Bluetooth. Esto es posible ya que en el Arduino se encuentra instalado un módulo BlueTooth que nos permite el intercambio de datos con el dispositivo Android.
- Una vez conectado, se puede deshabilitar la función automática que se describió anteriormente. Es entonces que el usuario puede controlar los diferentes componentes del vehículo. Ya sé por botones dispuestos en al interfaz gráfico, o por sensores, programados para que el manejo de los componentes se realice de una forma más cómoda y natural para el usuario.
- Así, se puede activar o desactivar el limpiaparabrisas, agitando el Smartphone con un "Shake". Cada uno de estos Shake envía la señal al Arduino, de que se cambie de estado el limpiaparabrisas.
- Deslizando la mano sobre el dispositivo, este detecta el cambio de proximidad en este y activa la marcha atrás, activando también, el sensor de proximidad.
- Los faros del vehículo, se activarán y desactivarán automáticamente. Captando la luz que ahora, es sensada por medio del dispositivo Android
- Finalmente, el usuario podrá visualizar en tiempo real, el estado de cada uno de los sensores (lluvia, luz y proximidad) por medio de la aplicación Android desarrollada
