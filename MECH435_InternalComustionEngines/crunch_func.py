import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def scale_data(value, minimum, maximum):
    return (value - minimum) * (180 / (maximum - minimum))

def scrub_data(df):
    df['div_crank'] = np.abs(np.diff(df['crank_deg'], prepend=df['crank_deg'].iloc[0]))
    df['crank_ang'] = ''
    df['stage'] = ''
    df['cycle'] = ''
    counter = 0
    cycle_count = 1
    for i in range(len(df['crank_deg'])):
        if counter == 4:
            counter = 0
            cycle_count += 1
        if df.loc[i, 'div_crank'] > 100:
            counter += 1
        df.loc[i, 'stage'] = counter
        df.loc[i, 'cycle'] = cycle_count
    # Add 0 for piston in stage 0 and 1
    df.loc[(df['stage'] == 0) | (df['stage'] == 1), 'crank_ang']= df['crank_deg']
    # Add 360 for piston in stage 2 and 3
    df.loc[(df['stage'] == 2) | (df['stage'] == 3), 'crank_ang'] = df['crank_deg'] + 360
    # Add 720 for piston in stage 4
    df.loc[df['stage'] == 4, 'crank_ang']= df['crank_deg'] + 720
    return df

def scale_pressure(df):
    df['ref_p'] = ''
    df.loc[0,'ref_p'] = 1.01
    pre_p = 0
    for i in range(len(df['pressure_bar'])):
        df.loc[i,'ref_p'] = pre_p + df['pressure_bar'].iloc[i]
        pre_p = df['pressure_bar'].iloc[i]
    return df

def clean_data(df):
    df['pressure_bar'] = df['pressure_v'] * 10.2
    df_min = min(df["crank_v"])
    df_max = max(df["crank_v"])
    df['crank_deg'] = scale_data(df['crank_v'], df_min, df_max)
    df = scrub_data(df)
    df = scale_pressure(df)
    return df

def select_cycles(df, list):
    df = df[df['cycle'].isin(list)]
    return df

# Load Data
## Load data will look for cleaned data, if cleaned data does not exist, it will look for raw data. 
## Raw data will be cleaned and returned as a Pandas Dataframe. A csv file will also be created with cleaned data. 
## Parsing raw data will take about 30 seconds
def load_data(csv_name, txt_name):
    try:
        df = pd.read_csv(csv_name)
        print(f"Found {csv_name}, CSV converted into dataframe.")
        return df
    except FileNotFoundError:
        try:
            print(f"{csv_name} not found. Will try to load {txt_name} raw data, clean, and create new csv.")
            # Time [s], Crank Angle [V], Pressure [V]
            df = pd.read_csv(txt_name, sep='\t', names=['time', 'crank_v', 'pressure_v'])
            print(f"{txt_name} file found, processing data and converting to dataframe.")
            print("Returned as Pandas Dataframe.")
            return df
        except FileNotFoundError:
            print(f"{txt_name} raw data not found, please put {txt_name} file in same directory as this ipynb file.")
            return False