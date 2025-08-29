<!-- ===================== BANNER ===================== -->
<p align="center">
  <img src="assets/banner.svg" alt="Banner de TODO_TITULO" width="100%" />
</p>

<p align="center"><i>🧠 Philosophers (42 Cursus) — Solución al clásico "Dining Philosophers Problem" con threads y mutexes, evitando deadlocks race conditions y starvation.</i></p>

---

## Índice
- [Resumen](#resumen)
- [Capturas / Demo](#capturas--demo)
- [Stack / Dependencias](#stack--dependencias)
- [Estructura](#estructura)
- [Instalación y uso](#instalación-y-uso)
- [Tests](#tests)
---
## ✅ Resumen del proyecto<br>

Philosophers simula a N filósofos sentados alrededor de una mesa. Cada filósofo alterna entre pensar, comer y dormir. Para comer, necesita dos tenedores (izquierdo y derecho). Los tenedores son recursos compartidos, modelados con mutexes. El objetivo es que la simulación no entre en deadlock (todos esperando indefinidamente), evite starvation (nadie se queda sin comer indefinidamente), y mantenga la precisión de tiempos dada por los parámetros.

## 🧩 ¿Para qué?

**¿Qué se aprende?**
- Concurrencia en C con `pthreads`: creación/joineo de hilos, diseño de rutinas.
- **Sincronización** con `mutex`: proteger estados y serializar la salida.
- **Prevención de deadlocks** y **starvation** (orden par/impar, caso N=1, monitor).
- **Temporización en ms**: `gettimeofday` + sleep activo a intervalos cortos.
- Debug y calidad: manejo de errores, `valgrind`/sanitizers, destrucción ordenada de mutexes.

Puedes usar : https://github.com/dantonik/42-philosophers-tester Para probar

---
## 🛠️ Compilación
⬇️ Descarga

git clone https://github.com/NanoHtz/philosofers.git
cd philosofers

🏗️ Compilar con Makefile

make # compila el ejecutable philo
make re # limpia y recompila desde cero
make clean # borra objetos
make fclean # borra objetos y el binario

## ▶️ Uso
## 🖨️ Salida esperada y pruebas
