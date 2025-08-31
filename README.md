<!-- ===================== BANNER ===================== -->
![Philosophers banner](https://raw.githubusercontent.com/NanoHtz/assets/main/philosophers/banner.svg)

<p align="center"><i>🧠 Philosophers (42 Cursus) — Solución al clásico "Dining Philosophers Problem" con threads y mutexes, evitando deadlocks race conditions y starvation.</i></p>

---

## Índice
- [Resumen](#resumen)
- [¿Para qué?](#para-que)
- [Explicación](#explicacion)
- [Compilación](#compilacion)
- [Uso](#uso)
- [Salida esperada y pruebas](#salida)

---
<a id="resumen"></a>
## ✅ Resumen del proyecto<br>

Philosophers simula a N filósofos sentados alrededor de una mesa. Cada filósofo alterna entre pensar, comer y dormir. Para comer, necesita dos tenedores (izquierdo y derecho). Los tenedores son recursos compartidos, modelados con mutexes. El objetivo es que la simulación no entre en deadlock (todos esperando indefinidamente), evite starvation (nadie se queda sin comer indefinidamente), y mantenga la precisión de tiempos dada por los parámetros.

<a id="para-que"></a>
## 🧩 ¿Para qué?

**¿Qué se aprende?**
- Concurrencia en C con `pthreads`: creación/joineo de hilos, diseño de rutinas.
- **Sincronización** con `mutex`: proteger estados.
- **Prevención de deadlocks** y **starvation**.
- **Temporización en ms**: `gettimeofday` + sleep activo a intervalos cortos.
---

<a id="explicacion"></a>
<details>
  <summary><h3>📝 Explicación</h3></summary>

En este proyecto nos centramos en los hilos.

  <!-- Pon aquí tu explicación técnica: 
       - Modelo de hilos: un hilo por filósofo + 1 monitor (si usas monitor).
       - Política de tomar tenedores (izq/dcha) y cómo evitas deadlock.
       - Estructuras: estado compartido, mutex por tenedor, mutex de impresión, etc.
       - Temporización: cómo mides ms y cómo haces sleeps cortos activos.
       - Criterios de fin: muerte, must_eat, orden de salida.
  -->

</details>

---

<a id="compilacion"></a>
## 🛠️ Compilación
⬇️ Descarga

```bash
git clone https://github.com/NanoHtz/philosofers.git
cd philosofers
```

🏗️ Compilar con Makefile

```bash
make # compila el ejecutable philo
make re # limpia y recompila desde cero
make clean # borra objetos
make fclean # borra objetos y el binario
```
<a id="uso"></a>
## ▶️ Uso

```bash
./philo number_of_philosophers(>=1) time_to_die(ms) time_to_eat(ms) time_to_sleep(ms) [must_eat]
```
Donde:<br>
- number_of_philosophers: número de filósofos que participaran(≥1)<br>
- time_to_die: tiempo máximo desde la última comida hasta morir(ms).<br>
- time_to_eat: tiempo que tardan en comer (con 2 tenedores)(ms).<br>
- time_to_sleep: tiempo que duermen tras comer(ms).<br>
- must_eat (opcional): si se da, la simulación termina cuando cada filósofo ha comido al menos ese número de veces.<br>

<a id="salida"></a>
## 🖨️ Salida esperada y pruebas

Sale en este formato:

```bash
<timestamp_ms> <id_filo> <acción>
```
Ejemplo:
```bash
0 1 is thinking
1 2 is thinking
2 1 has taken a fork
3 1 has taken a fork
3 1 is eating
...
803 4 died
```
Pruebas:
Te dejo algunos test básicos para que puedas probar:
```bash
./philo									-> Faltan argumentos; debe imprimir error por stderr
./philo 1 2 3							-> Faltan argumentos; debe imprimir error por stderr
./philo foo 800 200 200					-> Entrada no numérica; debe rechazar con “Only digits” por stderr
./philo -5 800 200 200					-> Signo no permitido si exiges “solo dígitos”; debe rechazar por stderr
./philo 0 800 200 200					-> Valor fuera de rango (número de filósofos debe ser ≥ 1); error por stderr
./philo 5 800 200 200 300 400			-> Demasiados argumentos; error por stderr
./philo 5 0 200 200						-> time_to_die debe ser > 0; error de rango por stderr
./philo 5 2147483647 200 200			-> Límite superior válido; la simulación arranca correctamente
./philo 5 2147483648 200 200			-> Overflow / fuera de rango; error por stderr y exit code ≠ 0
./philo " 5" 800 200 200				-> Entrada con espacio; según tu política, normalmente error “Only digits”
./philo 2 800 200 200					-> Sin must_eat; nadie muere y la simulación no termina sola
./philo 1 300 100 100					-> Caso 1 filósofo; puede “has taken a fork” y luego un único “died” (~300 ms);
./philo 5 10 100 100					-> time_to_die muy bajo; debe aparecer un único “died” y ser la última línea
./philo 50 1000 100 100					-> Escalabilidad; muchos hilos, sin muertes y sin terminar solo
./philo 50 1000 100 100 1				-> must_eat=1; termina cuando todos comen una vez; cero “died”
./philo 5 800 200 200 3					-> Debe finalizar cuando todos coman 3 veces; cero “died”;
./philo 5 310 200 200					-> Debe morir uno alrededor de 310 ms (± margen);
./philo 5 200 300 100					-> Caso límite (eat > die); debe morir alguien (un único “died”)
./philo 5 10 100 100					-> Muerte muy rápida; un “died” y es la última línea
```
Si pasas estas pruebas satisfactoriamente te animo a utilizar el tester, realiza pruebas de deadlock, starvation y demas:
- https://github.com/dantonik/42-philosophers-tester

Valgrind:
```bahs
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./philo 5 800 200 200 3
valgrind --tool=helgrind ./philo 5 800 200 200 3
valgrind --tool=drd ./philo 5 800 200 200 3
```

