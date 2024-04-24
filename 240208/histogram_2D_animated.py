#!/usr/bin/env python3
import sys
sys.dont_write_bytecode = True
import os
from scipy.interpolate import CubicSpline, PchipInterpolator, make_interp_spline
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import math

def create_default_plot():
    fig, ax = plt.subplots(1, 1, squeeze=False, figsize=(10, 9))
    ax = ax.flatten()
    ax[0].set_axisbelow(True)
    ax[0].grid(visible=True, axis='both', color='gray', linestyle=':', linewidth=1)
    return fig, ax[0]

# For plotting photoelectron number. That is input data is assumed to be integer and
# thus logarithmic binning is rounded.
def plot_2d_histogram(axis, xy_data, x_bins_info = (0, 100, 100, False), y_bins_info = (0, 100, 100, False), alpha = 1.0):
    x_min, x_max, x_N, x_log = x_bins_info
    y_min, y_max, y_N, y_log = y_bins_info
    if x_log and x_min <= 0:
        x_min = np.exp(np.log10(x_max) / (x_N+1))
    if y_log and y_min <= 0:
        y_min = np.exp(np.log10(y_max) / (y_N+1))
    xs, ys = xy_data

    x_bins = np.logspace(np.log10(x_min), np.log10(x_max), x_N)
    x_bins = [round(b) for b in x_bins if round(b) > 0]
    x_bins = ([x_min] if x_min < x_bins[0] else []) + list(dict.fromkeys(x_bins))

    y_bins = np.logspace(np.log10(y_min), np.log10(y_max), y_N)
    y_bins = [round(b) for b in y_bins if round(b) > 0]
    y_bins =([y_min] if y_min < y_bins[0] else []) + list(dict.fromkeys(y_bins))

    return axis.hist2d(xs, ys, bins=(x_bins, y_bins), range=[[x_min, x_max], [y_min, y_max]], cmap=plt.cm.plasma, cmin=1, alpha=alpha)

def load_877V():
    raw_data = np.loadtxt("results_v1/X-ray_14mm_coll_877V/10_SiPMs_vs_slowPMTs_Npe_no_cuts_0.txt",
                          dtype = {'names' : ('event_number', 'PMTs_Npe', 'SiPMs_Npe'),
                                   'formats': ('u4', 'f8', 'f8')})
    return {
        'histogram_title' : "X-ray 14mm coll 877V SiPMs vs PMTs",
        'output_fname': "results_v1/X-ray_14mm_coll_877V/10_SiPMs_vs_slowPMTs_Npe_no_cuts_0.mp4",
        'evts': raw_data['event_number'],
        'xs': raw_data['PMTs_Npe'],
        'ys': raw_data['SiPMs_Npe'],
        'x_binning': (1, 5000, 500, True),
        'y_binning': (0.1, 20000, 1000, True)
    }

def load_831V():
    raw_data = np.loadtxt("results_v1/X-ray_14mm_coll_831V/10_SiPMs_vs_slowPMTs_Npe_no_cuts_0.txt",
                          dtype = {'names' : ('event_number', 'PMTs_Npe', 'SiPMs_Npe'),
                                   'formats': ('u4', 'f8', 'f8')})
    return {
        'histogram_title' : "X-ray 14mm coll 831V SiPMs vs PMTs",
        'output_fname': "results_v1/X-ray_14mm_coll_831V/10_SiPMs_vs_slowPMTs_Npe_no_cuts_0.mp4",
        'evts': raw_data['event_number'],
        'xs': raw_data['PMTs_Npe'],
        'ys': raw_data['SiPMs_Npe'],
        'x_binning': (1, 4000, 500, True),
        'y_binning': (0.1, 20000, 1000, True)
    }

def load_785V():
    raw_data = np.loadtxt("results_v1/X-ray_14mm_coll_785V/10_SiPMs_vs_slowPMTs_Npe_no_cuts_0.txt",
                          dtype = {'names' : ('event_number', 'PMTs_Npe', 'SiPMs_Npe'),
                                   'formats': ('u4', 'f8', 'f8')})
    return {
        'histogram_title' : "X-ray 14mm coll 785V SiPMs vs PMTs",
        'output_fname': "results_v1/X-ray_14mm_coll_785V/10_SiPMs_vs_slowPMTs_Npe_no_cuts_0.mp4",
        'evts': raw_data['event_number'],
        'xs': raw_data['PMTs_Npe'],
        'ys': raw_data['SiPMs_Npe'],
        'x_binning': (1, 4000, 500, True),
        'y_binning': (0.1, 20000, 1000, True)
    }

def load_646V():
    raw_data = np.loadtxt("results_v1/X-ray_14mm_coll_646V/10_SiPMs_vs_slowPMTs_Npe_no_cuts_0.txt",
                          dtype = {'names' : ('event_number', 'PMTs_Npe', 'SiPMs_Npe'),
                                   'formats': ('u4', 'f8', 'f8')})
    return {
        'histogram_title' : "X-ray 14mm coll 646V SiPMs vs PMTs",
        'output_fname': "results_v1/X-ray_14mm_coll_646V/10_SiPMs_vs_slowPMTs_Npe_no_cuts_0.mp4",
        'evts': raw_data['event_number'],
        'xs': raw_data['PMTs_Npe'],
        'ys': raw_data['SiPMs_Npe'],
        'x_binning': (1, 4000, 500, True),
        'y_binning': (0.1, 20000, 1000, True)
    }

def load_600V():
    raw_data = np.loadtxt("results_v1/X-ray_14mm_coll_600V/10_SiPMs_vs_slowPMTs_Npe_no_cuts_0.txt",
                          dtype = {'names' : ('event_number', 'PMTs_Npe', 'SiPMs_Npe'),
                                   'formats': ('u4', 'f8', 'f8')})
    return {
        'histogram_title' : "X-ray 14mm coll 600V SiPMs vs PMTs",
        'output_fname': "results_v1/X-ray_14mm_coll_600V/10_SiPMs_vs_slowPMTs_Npe_no_cuts_0.mp4",
        'evts': raw_data['event_number'],
        'xs': raw_data['PMTs_Npe'],
        'ys': raw_data['SiPMs_Npe'],
        'x_binning': (1, 6000, 600, True),
        'y_binning': (0.1, 20000, 1000, True)
    }

def load_185V():
    raw_data = np.loadtxt("results_v1/X-ray_14mm_coll_185V/10_SiPMs_vs_slowPMTs_Npe_no_cuts_0.txt",
                          dtype = {'names' : ('event_number', 'PMTs_Npe', 'SiPMs_Npe'),
                                   'formats': ('u4', 'f8', 'f8')})
    return {
        'histogram_title' : "X-ray 14mm coll 185V SiPMs vs PMTs",
        'output_fname': "results_v1/X-ray_14mm_coll_185V/10_SiPMs_vs_slowPMTs_Npe_no_cuts_0.mp4",
        'evts': raw_data['event_number'],
        'xs': raw_data['PMTs_Npe'],
        'ys': raw_data['SiPMs_Npe'],
        'x_binning': (1, 5000, 500, True),
        'y_binning': (0.1, 10000, 1000, True)
    }

def load_16kV():
    raw_data = np.loadtxt("results_v1/X-ray_14mm_coll_V0_16kV/10_SiPMs_vs_slowPMTs_Npe_no_cuts_0.txt",
                          dtype = {'names' : ('event_number', 'PMTs_Npe', 'SiPMs_Npe'),
                                   'formats': ('u4', 'f8', 'f8')})
    return {
        'histogram_title' : "X-ray 14mm coll 16kV SiPMs vs PMTs",
        'output_fname': "results_v1/X-ray_14mm_coll_V0_16kV/10_SiPMs_vs_slowPMTs_Npe_no_cuts_0.mp4",
        'evts': raw_data['event_number'],
        'xs': raw_data['PMTs_Npe'],
        'ys': raw_data['SiPMs_Npe'],
        'x_binning': (1, 5000, 500, True),
        'y_binning': (0.1, 10000, 1000, True)
    }


def plot_SiPM_vs_PMT(data):
    histogram_title = data['histogram_title']
    output_fname = data['output_fname']
    evts, xs, ys = data['evts'], data['xs'], data['ys']
    x_binning = data['x_binning']
    y_binning = data['y_binning']
    my_cmap = plt.cm.plasma
    my_cmap.set_under('w', 1)

    fig, ax = create_default_plot()

    ax.set_title(histogram_title)
    ax.set_xlabel(fr"PMTs N$_{{PE}}$")
    ax.set_ylabel(fr"SiPMs N$_{{PE}}$")
    ax.set_yscale('log')
    ax.set_xscale('log')

    plot_2d_histogram(ax, (xs, ys), x_bins_info=x_binning, y_bins_info=y_binning)

    fig, ax = create_default_plot()
    ax.set_title(histogram_title)
    ax.set_xlabel(fr"PMTs N$_{{PE}}$")
    ax.set_ylabel(fr"SiPMs N$_{{PE}}$")
    ax.set_yscale('log')
    ax.set_xscale('log')

    plot_2d_histogram(ax, ([], []), x_bins_info=x_binning, y_bins_info=y_binning)

    def prepare_data_slicing():
        max_step_N = 50
        min_step_N = 1
        current_step = max_step_N
        current_N = 0
        prev_disp_x = 0
        prev_disp_y = 0
        prev_avg_x = None
        prev_avg_y = None
        max_N = len(evts) - 1
        i = 0
        data_slicing = []
        while True:
            current_step = min(current_step * 3, max_step_N)
            while True:
                next_N = min(current_N + current_step, max_N)
                X = xs[current_N : next_N]
                Y = ys[current_N : next_N]

                avg_pt_x = np.average(X)
                avg_pt_y = np.average(Y)
                disp_pt_x = np.std(X)
                disp_pt_y = np.std(Y)

                if not prev_avg_x or not prev_avg_y or current_step == min_step_N:
                    break
                if abs(avg_pt_x - prev_avg_x) < 1.4*max(disp_pt_x, prev_disp_x) and abs(avg_pt_y - prev_avg_y) < 1.4*max(disp_pt_y, prev_disp_y):
                    break
                if abs(avg_pt_x - prev_avg_x) > 4.0*max(disp_pt_x, prev_disp_x) or abs(avg_pt_y - prev_avg_y) > 4.0*max(disp_pt_y, prev_disp_y):
                    current_step = max(round(current_step/2.0), min_step_N)
                    continue
                if 1.3 * prev_disp_x > disp_pt_x and 1.3 * prev_disp_y > disp_pt_y:
                    break
                current_step = max(round(current_step/2.0), min_step_N)

            prev_disp_x = disp_pt_x if disp_pt_x > 0 else prev_disp_x
            prev_disp_y = disp_pt_y if disp_pt_y > 0 else prev_disp_y
            prev_avg_x = avg_pt_x
            prev_avg_y = avg_pt_y

            data_slicing.append((current_N, next_N))
            current_N = next_N + 1
            i += 1

            if next_N == max_N:
                return data_slicing

    # initializing empty values 
    # for x and y co-ordinates 
    # of running average
    average_x_points, average_y_points = [], []
    data_slicing = prepare_data_slicing()

    def animate_last_frame():
        ax.clear()
        ax.set_title(histogram_title)
        ax.set_xlabel(fr"PMTs N$_{{PE}}$")
        ax.set_ylabel(fr"SiPMs N$_{{PE}}$")
        ax.set_yscale('log')
        ax.set_xscale('log')

        plot_2d_histogram(ax, (xs, ys), x_bins_info=x_binning, y_bins_info=y_binning)
        ax.plot(average_x_points, average_y_points, lw = 2, color='r')

    # Define animation.
    def animate(i):
        ax.clear()
        ax.set_title(histogram_title)
        ax.set_xlabel(fr"PMTs N$_{{PE}}$")
        ax.set_ylabel(fr"SiPMs N$_{{PE}}$")
        ax.set_yscale('log')
        ax.set_xscale('log')

        current_N, next_N = data_slicing[i]

        X = xs[current_N : next_N]
        Y = ys[current_N : next_N]

        average_x_points.append(np.average(X))
        average_y_points.append(np.average(Y))

        if next_N == len(evts) - 1:
            animate_last_frame()
            return

        X_acc = xs[0 : next_N]
        Y_acc = ys[0 : next_N]
        plot_2d_histogram(ax, (X, Y), x_bins_info=x_binning, y_bins_info=(0.1, 20000, 1000, True))
        plot_2d_histogram(ax, (X_acc, Y_acc), x_bins_info=x_binning, y_bins_info=(0.1, 20000, 1000, True), alpha = 0.3)
        ax.plot(average_x_points, average_y_points, lw = 2, color='r')

    anim = animation.FuncAnimation(fig, animate, len(data_slicing),
                          interval = 200, blit = False, repeat = False, cache_frame_data=False)
    
    anim.save(output_fname, writer = 'ffmpeg', fps = 10)
    average_x_points, average_y_points = [], []

    return anim


if __name__ == "__main__":

    anin1 = plot_SiPM_vs_PMT(load_877V())
    anin2 = plot_SiPM_vs_PMT(load_831V())
    anin3 = plot_SiPM_vs_PMT(load_785V())
    anin4 = plot_SiPM_vs_PMT(load_646V())
    anin5 = plot_SiPM_vs_PMT(load_600V())
    anin6 = plot_SiPM_vs_PMT(load_185V())
    anin7 = plot_SiPM_vs_PMT(load_16kV())

    plt.tight_layout()
    plt.show()
