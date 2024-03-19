import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def get_bc_list(df, consec_crankv, bound):
    cycles = 0
    max_consec_crank = 0
    # Getting Top Center Points
    bc_list = []
    v_count = 1
    for i in range(len(df)):
        if v_count > max_consec_crank:
            max_consec_crank = v_count
        if (v_count == 1) & (df['crank_v'].iloc[i] > 9):
            bc_start = df.index[i]
        if (df['crank_v'].iloc[i-1] > bound) & (df['crank_v'].iloc[i] > bound):
            v_count += 1
        if (v_count >= consec_crankv) & (df['crank_v'].iloc[i] > bound):
            bc_list.append(bc_start)
            v_count = 1
            cycles += 1
        if df['crank_v'].iloc[i] < bound:
            v_count = 1 
    if cycles == 0:
        print(f"{consec_crankv} consecutive {bound}V Crank angle readings not found")
        print(f"Longest recuring voltage reading above {bound} is {max_consec_crank}")
        print("Bottom Center Points Not Found")
        return []
    print("Bottom Center Points Found")
    return bc_list

def get_tc_list(df, consec_crankv, bound):
    cycles = 0
    max_consec_crank = 0
    # Getting Top Center Points
    tc_list = []
    v_count = 1
    for i in range(len(df)):
        if v_count > max_consec_crank:
            max_consec_crank = v_count
        if (v_count == 1) & (df['crank_v'].iloc[i] < bound):
            tc_start = df.index[i]
        if (df['crank_v'].iloc[i-1] < bound) & (df['crank_v'].iloc[i] < bound):
            v_count += 1
        if (v_count >= consec_crankv) & (df['crank_v'].iloc[i] > bound):
            tc_list.append(tc_start)
            v_count = 1
            cycles += 1
        if df['crank_v'].iloc[i] > bound:
            v_count = 1 
    if cycles == 0:
        print(f"{consec_crankv} consecutive {bound}V Crank angle readings not found")
        print(f"Longest recuring voltage reading below {bound} is {max_consec_crank}")
        print("Top Center Points Not Found")
        return []
    print("Top Center Points Found")
    return tc_list

def set_tc_states(df, tc_list):
    state = 1
    add_list = []
    for i in range(len(tc_list)):
        index = tc_list[i]
        if state == 1:
            df.loc[index, 'crank_state'] = 5
            df.loc[index, 'rel_crank_ang'] = 720
            df.loc[index + 1, 'crank_state'] = state
            df.loc[index + 1, 'rel_crank_ang'] = 0
            add_list.append(index + 1)
            state = 3
            continue
        if state == 3:
            df.loc[index, 'crank_state'] = state
            df.loc[index, 'rel_crank_ang'] = 360
            state = 1
            continue
    tc_list += add_list
    tc_list.sort()
    return tc_list

def set_bc_states(df, bc_list):
    state = 2
    rel_crank_ang = 180
    for i in range(len(bc_list)):
        index = bc_list[i]
        df.loc[index, 'crank_state'] = state
        df.loc[index, 'rel_crank_ang'] = rel_crank_ang
        state += 2
        rel_crank_ang += 360
        if state > 5:
            state = 2
            rel_crank_ang = 180
    return bc_list

def create_avged_states(known_list):
    new_list = []
    for i in range(len(known_list)):
        if i == len(known_list) - 1: continue
        new_list.append((known_list[i] +  known_list[i+1])//2)
    return new_list

def scale_pressure(df):
    df['pressure_bar'] = df['pressure_v'] * 10.2
    return df

def set_crank_states(df, bc_consec, bc_bound, tc_consec, tc_bound):
    tc_list = get_tc_list(df, tc_consec, tc_bound)
    if len(tc_list) != 0:
        tc_list = set_tc_states(df, tc_list)
    
    bc_list = get_bc_list(df, bc_consec, bc_bound)
    if len(bc_list) != 0:
        bc_list = set_bc_states(df, bc_list)

# This permutation should never happen with the data that we have
    if (len(bc_list) == 0) & (len(tc_list) != 0):
        bc_list = create_avged_states(tc_list)
        bc_list = set_bc_states(df, bc_list)
        print("something went wrong, this code should not be called")

# This is the permutation that should be expected
    elif (len(tc_list) == 0) & (len(bc_list) != 0):
        tc_list = create_avged_states(bc_list)
        # Since we are taking bottom center as the start of the cycle, start state is state 2, thus tc start should be 3.
        state = 3
        add_list = []
        for i in range(len(tc_list)):
            index = tc_list[i]
            if state == 1:
                df.loc[index, 'crank_state'] = 5
                df.loc[index, 'rel_crank_ang'] = 720
                df.loc[index + 1, 'crank_state'] = state
                df.loc[index + 1, 'rel_crank_ang'] = 0
                add_list.append(index + 1)
                state = 3
                continue
            if state == 3:
                df.loc[index, 'crank_state'] = state
                df.loc[index, 'rel_crank_ang'] = 360
                state = 1
                continue
        tc_list += add_list
        tc_list.sort()
        
    if (len(bc_list) == 0) & (len(tc_list) == 0):
        print("Could not get consitent voltage readings to find crank angle states.")
        return df
    
    state_list = tc_list + bc_list
    
    state_list.sort()
    # Interpolation of mid points for crank angles
    for i in range(len(state_list)):
        if i == len(state_list) - 1:
            continue
        index = state_list[i]
        ang1 =  df.loc[df.index[state_list[i]], 'rel_crank_ang']
        dp1 = state_list[i]
        ang2 = df.loc[df.index[state_list[i+1]], 'rel_crank_ang']
        dp2 = state_list[i+1]
        slope = (ang2 - ang1)/(dp2-dp1)
        points_to_inter = dp2 - dp1
        for inter_point in range(points_to_inter):
            inter_ang = ang2-(slope*(dp2 - (dp1 + inter_point)))
            df.loc[df.index[dp1 + inter_point],'rel_crank_ang'] = inter_ang
    print("Interpolated Relative Crank Angles")
    df = df.dropna(subset=['rel_crank_ang']) 
    
    # Calculating absolute crank angle
    for i in range(len(df)):
        index = df.index[i]
        if i == 0:
            df.loc[index, 'abs_crank_ang'] = 0
            continue
        if abs(df.loc[index, 'rel_crank_ang'] - df.loc[index-1, 'rel_crank_ang']) > 100:
            df.loc[index, 'abs_crank_ang'] = df.loc[index-1, 'abs_crank_ang']
            continue
        abs_ang = df.loc[index-1, 'abs_crank_ang'] + df.loc[index, 'rel_crank_ang'] - df.loc[index-1, 'rel_crank_ang']
        df.loc[index, 'abs_crank_ang'] = abs_ang
    print("Calculated Absolute Crank Angles")
    return df

def clean_data(df, bc_consec, bc_bound, tc_consec, tc_bound):
    df = scale_pressure(df)
    df = set_crank_states(df, bc_consec, bc_bound, tc_consec, tc_bound)
    return df

# Load Data
## Load data will look for cleaned data, if cleaned data does not exist, it will look for raw data. 
## Raw data will be cleaned and returned as a Pandas Dataframe. A csv file will also be created with cleaned data. 
## Parsing raw data will take about 30 seconds
def load_data(csv_name, txt_name):
    try:
        df = pd.read_csv(csv_name)
        print(f"Found {csv_name}, CSV converted into dataframe.")
        return df, True
    except FileNotFoundError:
        try:
            print(f"{csv_name} not found. Will try to load {txt_name} raw data.")
            # Time [s], Crank Angle [V], Pressure [V]
            df = pd.read_csv(txt_name, sep='\t', names=['time', 'crank_v', 'pressure_v'])
            print(f"{txt_name} file found, processing data and converting to dataframe.")
            print("Returned as Pandas Dataframe. Data not crunched")
            return df, False
        except FileNotFoundError:
            print(f"{txt_name} raw data not found, please put {txt_name} file in same directory as this ipynb file.")
            return False, False