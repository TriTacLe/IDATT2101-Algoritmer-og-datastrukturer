import random
import timeit

"""
Helpers
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

def generate_random_lst(total_lst: int, lst_length) -> list:
  """
  Unused
  """
  lst = []
  for i in range(total_lst):
    lst.append(get_random_int_lst(lst_length=lst_length))
  return lst

"""
Oppgave 1-1
"""
def get_highest_return(data: list) -> int | None:
  """Algoritme som finner hvilket kjøps- og salgstidspunkt som lønner seg best. 
  """
  highest_return = 0
  entry_day = 0
  sell_day = 0
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
      current_return+=val 
      
    # Save highest return
    ## 1 sammenligning og 3 tilordninger
    if current_return > highest_return: 
      highest_return = current_return 
      entry_day = current_idx
      sell_day = i + 1
  ## Return operasjon
  return highest_return, entry_day, sell_day

def oppgave1():
  # kursforandringer oppgitt i oppgaven
  data = [-1, 3, -9, 2, 2, -1, 2, -1, -5]
  h, b, s = get_highest_return(data)
  print(f"Max return: {h}, entry on day: {b}, and sell on day: {s}")

"""
Oppgave 1-2. 
Kommentarer i funksjon "oppgave1" relatert til denne oppgaven er markert med ##. Markerer alle operasjoner, til å med enkle.

Løkka utføres n ganger (input størrelsen varierer og er ikke konstant) og går over alle elementene en gang. Det finnes derfor en konstant c1 slik at 0 <= f(n) <= c1*g(n) og f(n) er i O(n), g(n) = n
Hver iterasjon gjør et konstant antall operasjoner uansett og hopper ikke over noen indekser i loopen. Dermed fins det en konstant c2 slik at 0 <= c*g(n) <= f(n) og f(n) er i Ω(n), g(n) = n
O(n) og Ω(n) = Θ(n) da man må uansett gå igjennom alle elementene i lista og øvre og nedre grense er lineære. f(n) er i Θ(n)
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
  n_values = [1000, 10000, 100000, 1000000, 10000000]
  for n in n_values:
    lst = get_random_int_lst(n)
    time = timer_tracking(lst, 5)
    print(f"n_value: {n}, time: {time}")

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
  user_input = input("Choose oppgave\n").strip()
  try:
    choice = int(user_input)
  except ValueError:
    print("choose a number between 1 and 3\n")
  else:
    oppgave_chooser(choice)
  