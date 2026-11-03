# Taller Impala - Recreacion 3D en OpenGL

Proyecto final de Laboratorio de Computacion Grafica (Ing. Eden Espinoza Urzua).
Recreacion de un taller mecanico automotriz ("Servicio Automotriz") a partir de fotografias reales, hecha en OpenGL 3.3
con GLFW, GLEW, GLM, SOIL2 y Assimp sobre el codigo base proporcionado por el profesor.

## Como ejecutarlo

1. Abre la carpeta `ProyectoFinal/`.
2. Ejecuta `ProyectoFinal.exe` con doble clic (32 bits). No necesita Visual Studio.
3. Si falta alguna DLL, instala el Visual C++ Redistributable de 32 bits (x86) de Microsoft.

El ejecutable debe permanecer junto a las carpetas `Models` y `Shader` y a sus DLL.

## Como compilarlo

Abre `ProyectoFinal.slnx` en Visual Studio, elige **Release** y **x86** y compila la solucion.

## Controles

| Tecla | Accion |
|---|---|
| W / A / S / D o flechas | Mover la camara |
| Mouse | Girar la vista |
| 1 | Abrir / cerrar la puerta corrediza |
| 2 / 3 | Abrir / cerrar la puerta batiente A / B |
| 4 | Abrir / cerrar todas las puertas |
| Barra espaciadora | Encender / apagar la luz interior |
| F | Linterna |
| G | Animacion del gato hidraulico |
| M | Animacion del mecanico (reproducir / pausa) |
| Esc | Salir |

## Animaciones (todas por codigo)

- Puerta corrediza y puertas batientes (se abren hacia adentro).
- Ventilador con aspas girando siempre.
- Gato hidraulico levantando el auto (keyframes con modelado jerarquico).
- Mecanico trabajando (keyframes con modelado jerarquico).
- Dos autos circulando en sentidos opuestos por la calle.

## Contenido de la escena

Taller, banqueta y calle, arbol, perro, mesa de trabajo, caja de herramientas y gabinete con herramientas,
extintores, llanta, llave de cruz, elevador, Mercedes, sedan, mecanico, trabajadores, ventilador, cinco casas y cielo.

## Estructura

```
ProyectoFinal/
|-- Documentacion/       manuales de usuario y tecnico
|-- Referencias/         fotografias de referencia
|-- External Libraries/  GLEW, GLFW y Assimp
`-- ProyectoFinal/       codigo, Shader, Models, ejecutable y DLL
```
