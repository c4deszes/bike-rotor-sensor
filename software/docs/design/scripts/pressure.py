import math

# Constants
M = 0.0289644  # molar mass of Earth's air in kg/mol
g = 9.80665  # gravitational acceleration in m/s²
R = 8.31432  # universal gas constant in J/(mol·K)

def altcalc(a, k, i):
    if (a / i) < (101325 / 22632.1):
        d = -0.0065  # temperature lapse rate in K/m
        e = 0  # base altitude for this layer in meters
        j = math.pow((i / a), (R * d) / (g * M))
        return e + ((k * ((1 / j) - 1)) / d)
    elif (a / i) < (101325 / 5474.89):
        e = 11000  # base altitude for the next layer in meters
        b = k - 71.5  # temperature for the isothermal layer in K
        f = (R * b * math.log(i / a)) / ((-g) * M)
        l = 101325
        c = 22632.1
        h = ((R * b * math.log(l / c)) / ((-g) * M)) + e
        return h + f
    return float('nan')  # return NaN if conditions aren't met

def altitude_to_pressure(sea_level_pressure, temperature, altitude):
    if altitude < 11000:
        return sea_level_pressure * math.pow(1 - 0.0065 * altitude / temperature, 5.255)
    elif altitude < 20000:
        return 22632.1 * math.exp(-0.0001577 * (altitude - 11000))
    else:
        return float('nan')  # return NaN if conditions aren't met

# Example usage
sea_level_pressure = 103000  # Example input in Pa
temperature = 273.15 + 20  # Example input in K
#pressure = 101454  # Example input in Pa
pressure = 101530

altitude = altcalc(sea_level_pressure, temperature, pressure)
print(f"Altitude: {altitude:.2f} m")

pressure = altitude_to_pressure(sea_level_pressure, temperature, altitude)
print(f"Pressure: {pressure:.2f} Pa")



# sea level: 98000 - 104000 (1000 steps) -> 60
# temperature 263.15 - 323.15 (-10C - 50C) (10C steps) -> 7
# altitudes -500m - 5000m (100m steps) -> 55 * 16bit

#Generate lookup table
# lookup_table = []

# sea_level_pressures = range(100000, 104001, 100)
# temperatures = range(263, 324, 10)
# altitudes = range(-500, 5001, 100)
# pressure_offset = 48000

# for sea_level_pressure in sea_level_pressures:
#     table_2d = []
#     for temperature in temperatures:
#         table_1d = []
#         for altitude in altitudes:
#             pressure = altitude_to_pressure(sea_level_pressure, temperature, altitude)
#             table_1d.append(int(pressure) - pressure_offset)
#         table_2d.append(table_1d)
#     lookup_table.append(table_2d)

# # Convert to C array format
# c_array = f"uint16_t ALT_Lookup[{len(lookup_table)}][{len(lookup_table[0])}][{len(lookup_table[0][0])}] = {{\n"
# c_array += f"  /*  */\n"
# for i, table_2d in enumerate(lookup_table):
#     c_array += f"  {{ /* QNH={sea_level_pressures[i]} Pa */\n"
#     for j, table_1d in enumerate(table_2d):
#         c_array += f" /* T={temperatures[j]}K ({temperatures[j] - 273.15:.01f}C) */   {{" + ", ".join(f"{pressure}u" for pressure in table_1d) + "},\n"
#     c_array += "  },\n"
# c_array += "};"

# print(c_array)

