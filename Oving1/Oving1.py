import random
import timeit
import matplotlib.pyplot as plt 

"""
Helper
"""
def get_random_int_lst(lst_length: int) -> list:
  """_summary_
  Returns a random list of interger with n (argument) length
  Returns:
      _type_: _description_
  """
  lst = []
  for i in range(lst_length):
    random_numb = random.randint(-10,10)
    lst.append(random_numb)
  return lst

"""
Oppgave 1-1
"""
def get_highest_return(data: list):
  """Algoritme som finner hvilket kjøps- og salgstidspunkt som lønner seg best. 
  """
  ## Tilordninger
  highest_return = 0
  entry_day = 0
  sell_day = 1
  current_return = 0
  current_idx = 0
  for i, val in enumerate(data): ## Kjører n ganger
    # Reset return if negative (we do not want to start on minus)
    ## 1 sammenligning og 2 tilordninger
    if current_return <= 0: 
      current_return = val 
      current_idx = i 
    # Calculate return (avkastning)
    else:
      ## Addisjon + tilordning
      current_return+=  val 
  
    # Save highest return
    ## 1 sammenligning og 3 tilordninger
    if current_return > highest_return: 
      highest_return = current_return 
      entry_day = current_idx
      sell_day = i+1
  ## Return operasjon
  return highest_return, entry_day, sell_day 

def oppgave1():
  # kursforandringer oppgitt i oppgaven
  data = [-1, 3, -9, 2, 2, -1, 2, -1, -5]
  print(f"Aksjeendringer: {data}")
  h, b, s = get_highest_return(data)
  print(f"Max return: {h}, entry on day: {b}, and sell on day: {s}")

"""
Oppgave 1-2. 
Kommentarer i funksjon "oppgave1" relatert til denne oppgaven er markert med ##. Markerer alle operasjoner, til å med enkle operasjoner.

Løkka utføres n ganger (input størrelsen varierer og er ikke konstant) og går over alle elementene en gang. Det finnes derfor en konstant c1 slik at 0 <= f(n) <= c1*g(n) og f(n) er i O(n), g(n) = n
Hver iterasjon gjør et konstant antall operasjoner uansett og hopper ikke over noen indekser i loopen. Dermed fins det en konstant c2 slik at 0 <= c*g(n) <= f(n) og f(n) er i Ω(n), g(n) = n
O(n) og Ω(n) = Θ(n) da man må uansett gå igjennom alle elementene i lista og øvre og nedre grense er begge lineære. f(n) er i Θ(n)
Fra asymptotisk analyse telles ikke konstante faktorer med, så if- og else-setningene i funksjonen ovenfor påvirker bare konstantene c1 og c2.
"""

def oppgave2():
  print("""Kommentarer i funksjon "oppgave1" relatert til denne oppgaven er markert med ##. Markerer alle operasjoner, til å med enkle. \n
Løkka utføres n ganger (input størrelsen varierer og er ikke konstant) og går over alle elementene en gang. Det finnes derfor en konstant c1 slik at 0 <= f(n) <= c1*g(n) og f(n) er i O(n), g(n) = n
Hver iterasjon gjør et konstant antall operasjoner uansett og hopper ikke over noen indekser i loopen. Dermed fins det en konstant c2 slik at 0 <= c*g(n) <= f(n) og f(n) er i Ω(n), g(n) = n
O(n) og Ω(n) = Θ(n) da man må uansett gå igjennom alle elementene i lista og øvre og nedre grense er lineære. f(n) er i Θ(n)
Fra asymptotisk analyse telles ikke konstante faktorer med, så if- og else-setningene i funksjonen ovenfor påvirker bare konstantene c1 og c2""")

"""
Oppgave 1-3. 
"""
def timer_tracking(lst: list, rounds: int) -> float:
  t1 = timeit.default_timer()
  for _ in range(rounds):
    get_highest_return(lst)
  t2 = timeit.default_timer()
  return (t2-t1)/rounds

def oppgave3():
  n_values = [10**e for e in range(3,8)]
  print(f"tallrekkeverdier: {n_values}")
  time_values = []
  for n in n_values:
    lst = get_random_int_lst(n)
    time_per_run = timer_tracking(lst, len(n_values))
    time_values.append(time_per_run)
    print(f"n_value: {n}, time: {time_per_run} seconds")
  
  # Plotting
  #plt.plot(time_values, n_values)
  plt.scatter(time_values, n_values)
  plt.xlabel("time")
  plt.ylabel("n_values")
  plt.show()

"""
Målingen viser en lineær forhold mellom tid og størrelsen på n. Algoritmen er som nevnt ovenfor er algortmen Θ(n) og dermed lineær. 
n-verdiene valgt er en tidobling av n > 1000. I terminaloutput og plottingen ser vi at en tidobling av n, gir omtrent tidobling av kjøretid. 
Dette bekrefter min analyse i oppgave 1-2 at algoritmen er Θ(n)

Terminaloutput:
tallrekkeverdier: [1000, 10000, 100000, 1000000, 10000000]
n_value: 1000, time: 9.637200273573398e-05 seconds
n_value: 10000, time: 0.0009391811909154058 seconds
n_value: 100000, time: 0.00985900501254946 seconds
n_value: 1000000, time: 0.09542412641458213 seconds
n_value: 10000000, time: 1.0165447901934386 seconds
"""

def oppgave_chooser(number: int) -> None:
  match number:
    case 1:
      oppgave1()
    case 2:
      oppgave2()
    case 3:
      oppgave3()
    case _:
      print("choose a number between 1 and 3")

if __name__ == "__main__":
  user_input = input("Choose oppgave (number between 1 and 3)\n").strip()
  try:
    choice = int(user_input)
  except ValueError:
    print("choose a number between 1 and 3\n")
  else:
    oppgave_chooser(choice)