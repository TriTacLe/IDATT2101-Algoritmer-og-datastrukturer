import matplotlib.pyplot as plt

load_factor = [0.5, 0.8, 0.9, 0.99, 1.0]

xL_time = [442977395.00, 651514123.00, 810875575.00, 1420730304.00, 3956886160.00]
xD_time = [300215172.00, 763833122.00, 1008997811.00, 2071122855.00, 4742596194.00]

yL_col  = [791876.00, 5515335.00, 12196938.00, 45372466.00, 255570433.00]
yD_col  = [823380.00, 4433429.00, 8931734.00, 29820973.00, 127184054.00]


plt.figure()
plt.plot(xL_time, yL_col, marker="o", label="Lineær probing")
plt.plot(xD_time, yD_col, marker="s", label="Dobbel hashing")
plt.xlabel("Tid")
plt.ylabel("Kollisjoner")
plt.title("Kollisjoner vs tid (metodene)")
plt.grid(True)
plt.legend()
plt.tight_layout()

# Kollisjoner vs lastfaktor
plt.figure()
plt.plot(load_factor, yL_col, marker="o", label="Lineær probing")
plt.plot(load_factor, yD_col, marker="s", label="Dobbel hashing")
plt.xlabel("Lastfaktor")
plt.ylabel("Kollisjoner")
plt.title("Kollisjoner vs lastfaktor")
plt.grid(True)
plt.legend()
plt.tight_layout()

plt.show()
