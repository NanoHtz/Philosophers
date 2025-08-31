<!-- ===================== BANNER ===================== -->
<p align="center">
  <img src="https://raw.githubusercontent.com/NanoHtz/Assets/main/philosophers/banner.svg" alt="Philosophers banner">
</p>

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
<br>
Un hilo es la ejecucion minima dentro de un proceso, los hilos dentro de un proyecto, comparten recursos, memoria, descriptores de archivo etc...
<br>
En contra partida permiten hacer varias cosas a la vez, trabajar en paralelo sobre un mismo proceso, sirven de proteccion, si un hilo fuese muy lento, no se quedaria el trabajo paralizado, otros seguirian trabajando.
<br>
Habrá que manejar race conditions, es decir situaciones donde los hilos manejas la misma memoria, por ejemplo, dos hilos imprimiendo en el mismo archivo, los resultados son impredecibles, memoria basura... etc
<br>
🔒 ¿Qué es un mutex y para qué sirve?
Un mutex (mutual exclusion) es un cerrojo que garantiza que solo un hilo a la vez entra en una “sección crítica” (un trozo de código que accede a datos compartidos).
Sin mutexes, dos hilos podrían modificar/imprimir/leer el mismo recurso a la vez → race conditions.

pthread_mutex_lock(&m);   // entrar a sección crítica
/* ... usar/leer/escribir recurso compartido ... */
pthread_mutex_unlock(&m); // salir de sección crítica

<br>

📦 Secciones críticas típicas en este proyecto

Tenedores: cada tenedor es un pthread_mutex_t. Para comer hay que bloquear dos.

Impresión: un único print_mutex evita que se mezclen líneas en la salida.

(Opcional) Estado global: proteger stop, last_meal, contadores, etc., con un mutex de estado.

<br>

🧵 Concurrencia vs. Paralelismo

Concurrencia: varias tareas “avanzan” en el tiempo solapándose (aunque sea con 1 CPU).

Paralelismo: varias tareas se ejecutan a la vez (multinúcleo).
En ambos casos necesitas sincronización (mutexes) para datos compartidos.

<br>

🍴 Mapeo del problema a primitivas

Filósofo → hilo que repite: pensar → tomar tenedores → comer → soltar → dormir.

Tenedor → mutex. Cada sitio de la mesa tiene uno.

Mesa/estado compartido → estructura con:

forks[] (array de mutexes),

print_mutex,

(opcional) state_mutex, stop, done_count, start_time, reglas y tiempos.

<br>

🛑 Deadlock: cómo se evita
Si todos los hilos intentan coger primero el mismo lado, pueden quedarse bloqueados en círculo (cada uno con 1 tenedor esperando el otro).
Solución simple y canónica: orden asimétrico par/impar

Filósofos pares: cogen primero el derecho, luego el izquierdo.

Filósofos impares: primero el izquierdo, luego el derecho.

if (philo->id % 2 == 0) {        // par
    lock(right_fork);
    print("has taken a fork");
    lock(left_fork);
    print("has taken a fork");
} else {                          // impar
    lock(left_fork);
    print("has taken a fork");
    lock(right_fork);
    print("has taken a fork");
}


Con esta asimetría se rompe el ciclo de espera y no aparece deadlock.
(Alternativas existen, pero esta basta para el mandatory.)
<br>

🥣 Comer de forma segura (y contar comidas)
Justo al empezar a comer, se actualiza el último bocado (last_meal) bajo protección (para que el monitor pueda comprobar vida/muerte con precisión). Tras time_to_eat, se sueltran ambos tenedores (unlock) y opcionalmente se incrementa meals_done para el criterio must_eat.
<br>

🖨️ Logging atómico
Formato típico: <timestamp_ms> <id> <acción>
Todos los printf pasan por una función print_log() que bloquea print_mutex, imprime y desbloquea.
Regla de oro: si stop ya está activo, no imprimir nada más salvo el único "died".
<br>

⏱️ Temporización fiable (ms) y “arranque sincronizado”

Usa un now_ms() basado en reloj monotónico.

Implementa un sleep cooperativo (p. ej. bucles con usleep(100) comprobando el tiempo y stop) para no pasarte de time_to_eat/sleep.

Barrera de inicio: fijar start_time = now + 100ms y hacer que cada hilo espere hasta ese instante. Así todos comienzan “a la vez” y los timestamp son consistentes.

<br>

🩺 Monitor de vida
Un hilo (o bucle en el main) recorre periódicamente a los filósofos:

Si now - last_meal > time_to_die → activa stop y imprime una sola línea "died" (bajo print_mutex), y termina.

Si existe must_eat y todos han llegado → activa stop y termina sin muertes.

<br>

🧊 Casos borde importantes

N = 1: el filósofo solo puede tomar un tenedor → nunca llega a comer y debe morir tras time_to_die.

time_to_eat > time_to_die: inevitablemente alguien morirá antes de completar un bocado; el monitor debe detectarlo con precisión.

Impresión: la línea "died" debe ser la última. No mezclarla con otros prints.

<br>

🧱 Estructuras típicas (orientativas)

t_rules: n, time_to_die, time_to_eat, time_to_sleep, must_eat (opcional).

t_table (compartido): pthread_mutex_t forks[n], print_mutex, (opcional state_mutex), volatile int stop, int done_count, long start_time.

t_philosopher: int id, punteros a left/right fork, long last_meal, int meals_done, pthread_t thread, puntero a t_table/t_rules.

<br>

🧭 Flujo general del programa

Validar argumentos (numéricos, rangos).

Inicializar mesa (forks, mutexes, reglas, start_time).

Crear filósofos (hilos) con su contexto (forks izq/der, tiempos).

Arranque sincronizado, loop de cada hilo, y monitor.

Join de todos los hilos.

Destroy de mutexes y free de memoria.

<br>

✅ Checklist rápido

 Nunca imprimes fuera de print_mutex.

 Solo hay un "died" y es la última línea.

 Deadlock evitado con par/impar (o política equivalente).

 N=1 tratado correctamente.

 Tiempos en ms con reloj monotónico y smart_usleep.

 Limpieza completa: join → destroy mutexes → free.

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

