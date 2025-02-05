from .runtime import Inifile, MPIPool
from .main import run_cosmosis
from .runtime.utils import underline
import time
import os
import yaml
import sys
import warnings
import contextlib

def pipeline_after(params, after_module, modules):
    """
    Insert item(s) in the pipeline after the given module.

    Parameters
    ----------
    params : Inifile
        The parameter file to modify
    after_module : str
        The name of the module after which to insert the new modules
    modules : list
        The list of modules to insert

    Returns
    -------
    None
    """
    pipeline = params['pipeline', 'modules']
    pipeline = pipeline.split()
    if isinstance(modules, str):
        modules = [modules]

    i = pipeline.index(after_module)

    for m in modules[::-1]:
        pipeline.insert(i+1, m)
    params.set('pipeline', 'modules', ' '.join(pipeline))

def pipeline_before(params, before_module, modules):
    """
    Insert item(s) in the pipeline before the given module.

    If before_module is "$" then the item is inserted at the end

    Parameters
    ----------
    params : Inifile
        The parameter file to modify
    before_module : str 
        The name of the module before which to insert the new modules
    modules : list
        The list of modules to insert
    """
    if isinstance(modules, str):
        modules = [modules]

    pipeline = params['pipeline', 'modules']
    pipeline = pipeline.split()
    i = pipeline.index(before_module)

    for m in modules[::-1]:
        pipeline.insert(i, m)
    params.set('pipeline', 'modules', ' '.join(pipeline))

def pipeline_replace(params, original_module, modules):
    """
    Replace an item in the pipeline with one or more new items.

    If the original module is not found then this will fail.

    Parameters
    ----------
    params : Inifile
        The parameter file to modify
    original_module : str
        The name of the module to replace
    modules : list
        The list of modules to insert

    Returns
    -------
    None
    """
    pipeline_after(params, original_module, modules)
    pipeline_delete(params, [original_module])

def pipeline_delete(params, modules):
    """
    Delete one or more items in the pipeline.

    Parameters
    ----------
    params : Inifile
        The parameter file to modify
    modules : list
        The list of modules to delete

    Returns
    -------
    None
    """
    if isinstance(modules, str):
        modules = [modules]
    pipeline = params['pipeline', 'modules']
    pipeline = pipeline.split()

    for m in modules:
        pipeline.remove(m)
    params.set('pipeline', 'modules', ' '.join(pipeline))

def pipeline_append(params, modules):
    """
    Append item(s) to the end of the pipeline.

    Parameters
    ----------
    params : Inifile
        The parameter file to modify
    modules : list
        The list of modules to append
    
    Returns
    -------
    None
    """
    pipeline = params['pipeline', 'modules']
    pipeline = pipeline.split()

    for m in modules:
        pipeline.append(m)
    params.set('pipeline', 'modules', ' '.join(pipeline))

def pipeline_prepend(params, modules):
    """
    Prepend item(s) to the start of the pipeline.

    Parameters
    ----------
    params : Inifile
        The parameter file to modify
    modules : list

    Returns
    -------
    """
    pipeline = params['pipeline', 'modules']
    pipeline = pipeline.split()

    for m in modules[::-1]:
        pipeline.insert(0, m)
    params.set('pipeline', 'modules', ' '.join(pipeline))


def apply_update(ini, update, is_params=False):
    """
    Apply an update to a parameters, value, or priors configuration.

    Parameters
    ----------
    ini : Inifile
        The values or priors file to update
    update : str
        The update to apply. This should be of the form:
        section.option=value
        sampler=sampler_name
        del section.option
        del section
    """
    if "=" in update:
        keys, value = update.split("=", 1)
        keys = keys.strip()
        value = value.strip()
        if keys == "sampler":
            if not is_params:
                raise ValueError("You can only set the sampler in the parameters file")
            ini.set("runtime", "sampler", value.strip())
        else:
            section, option = keys.split(".", 1)
            section = section.strip()
            option = option.strip()
            if not ini.has_section(section) and section != "DEFAULT":
                ini.add_section(section)
            ini.set(section, option, value)
    elif update.startswith("del"):
        cmd, keys = update.split(maxsplit=1)
        if cmd not in ["del", "delete"]:
            raise ValueError(f"Unknown command {cmd}")
        if "." in keys:
            section, option = keys.split('.', 1)
            section = section.strip()
            option = option.strip()
            ini.remove_option(section, option)
        else:
            keys = keys.strip().split()
            if len(keys) > 1:
                raise ValueError("Can only delete one section at a time")
            else:
                # delete entire section
                ini.remove_section(keys[0])
    else:
        raise ValueError(f"Unknown update {update}")

def apply_updates(ini, updates, is_params=False):
    """
    Apply a list of updates to a parameters, values, or priors configuration.

    Parameters
    ----------
    ini : Inifile
        The values or priors file to update
    updates : list
        See apply_update for details.
    
    Returns
    -------
    None
    """
    for update in updates:
        try:
            apply_update(ini, update, is_params=is_params)
        except:
            raise ValueError(f"Malformed update: {update}")
        

def apply_pipeline_update(ini, update):
    """
    Apply an update to the list of modules forming the pipeline.
    
    Available actions are:
    - after <module> <new_module> [<new_module> ...]
    - before <module> <new_module> [<new_module> ...]
    - replace <module> <new_module> [<new_module> ...]
    - del <module> [<module> ...]
    - append <new_module> [<new_module> ...]
    - prepend <new_module> [<new_module> ...]

    Parameters
    ----------
    ini : Inifile
        The parameters file to update
    update : list
        The update to apply.  This should be a list of strings
        of the form [action, module, *values]
        values can be a single string or a list of strings,
        which will be joined with spaces.

    """
    action = update[0]
    if action == "after":
        pipeline_after(ini, update[1], update[2:])
    elif action == "before":
        pipeline_before(ini, update[1], update[2:])
    elif action == "replace":
        pipeline_replace(ini, update[1], update[2:])
    elif action == "del" or action == "delete":
        pipeline_delete(ini, update[1:])
    elif action == "append":
        pipeline_append(ini, update[1:])
    else:
        raise ValueError(f"Unknown pipeline action {action}")


def apply_pipeline_updates(params, pipeline_updates):
    """
    Apply a list of pipeline updates.

    Parameters
    ----------
    params : Inifile
        The parameters file to update
    pipeline_updates : list
        The list of updates to apply.  Each update should be a list of strings.
        See apply_pipeline_update for details.

    Returns
    -------
    None
    """
    for update in pipeline_updates:
        apply_pipeline_update(params, update.split())



@contextlib.contextmanager
def temporary_environment(env):
    """
    Temporary set the environment variables in the given dictionary.
    Once the with block is exited the environment is restored to its
    original state.

    Parameters
    ----------
    env : dict
        A dictionary of environment variables to set

    Returns
    -------
    None
    """
    original_environment = os.environ.copy()
    try:
        os.environ.update(env)
        yield
    finally:
        os.environ.clear()
        os.environ.update(original_environment)

def set_output_dir(params, name, output_dir):
    """
    Modify a parameters file to set the output directory and file names to be
    based on the run name and output directory.

    Parameters
    ----------
    params : Inifile
        The parameters file to update
    name : str
        The name of the run
    output_dir : str
        The output directory to use

    Returns
    -------
    None
    """
    # Always override the output file and various other auxiliary files
    if not params.has_section("output"):
        params.add_section("output")
    if not params.has_section("test"):
        params.add_section("test")

    params.set("output", "filename", os.path.join(output_dir, f"{name}.txt"))
    params.set("test", "save_dir", os.path.join(output_dir, name))

    if params.has_section("multinest"):
        params.set("multinest", "multinest_outfile_root", os.path.join(output_dir, f"{name}.multinest"))
    if params.has_section("polychord"):
        params.set("polychord", "polychord_outfile_root", f"{name}.polychord")
        params.set("polychord", "base_dir", output_dir)

def build_run(name, run_info, runs, components, output_dir):
    """
    Generate a dictionary specifying a CosmoSIS run from a run_info dictioary.

    Parameters
    ----------
    name : str
        The name of the run
    run_info : dict
        A dictionary specifying the run.  This can have the following keys:
        - base : the name of the base parameter file OR
        - parent : the name of the parent run to use as a base (one of these is required)
        - params : a list of updates to apply to the parameters file (optional)
        - values : a list of updates to apply to the values file (optional)
        - priors : a list of updates to apply to the priors file (optional)
        - pipeline : a list of updates to apply to the pipeline (optional)
        - components : a list of components to include (optional)
        - env : a dictionary of environment variables to set (optional)
    runs : dict
        A dictionary of previously built runs
    components : dict
        A dictionary of previously built components
    output_dir : str
        The output directory to use

    Returns
    -------
    """
    run = run_info.copy()
    env_vars = run_info.get("env", {})
    run["env"] = env_vars


    # We set the environment both now, when reading the ini files,
    # so that any variable expansion is done, and also later when running
    with temporary_environment(env_vars):
        if "base" in run_info:
            params = Inifile(run_info["base"], print_include_messages=False)
        elif "parent" in run_info:
            try:
                parent = runs[run_info["parent"]]
            except KeyError:
                warnings.warn(f"Run {name} specifies parent {run_info['parent']} but there is no run with that name yet")
                return None
            params = Inifile(parent["params"], print_include_messages=False)
        else:
            warnings.warn(f"Run {name} specifies neither 'parent' nor 'base' so is invalid")
            return None

        # Build values file, which is mandatory
        if "parent" in run_info:
            values = Inifile(parent["values"], print_include_messages=False)
        else:
            values_file = params.get('pipeline', 'values')
            values = Inifile(values_file, print_include_messages=False)

        # Build optional priors file
        if "parent" in run_info:
            priors = Inifile(parent["priors"], print_include_messages=False)
        elif "priors" in params.options("pipeline"):
            priors_file = params.get('pipeline', 'priors')
            priors = Inifile(priors_file, print_include_messages=False)
        else:
            priors = Inifile(None)
        


        # Make a list of all the modifications to be applied
        # to the different bits of this pipeline

        param_updates = []
        value_updates = []
        prior_updates = []
        pipeline_updates = []

        # First from any generic components specified
        for component in run_info.get("components", []):
            component_info = components[component]
            param_updates.extend(component_info.get("params", []))
            value_updates.extend(component_info.get("values", []))
            prior_updates.extend(component_info.get("priors", []))
            pipeline_updates.extend(component_info.get("pipeline", []))

        # And then for anything specific to this pipeline
        param_updates.extend(run_info.get("params", []))
        value_updates.extend(run_info.get("values", []))
        prior_updates.extend(run_info.get("priors", []))
        pipeline_updates.extend(run_info.get("pipeline", []))

        # Now apply all the steps
        apply_updates(params, param_updates, is_params=True)
        apply_updates(values, value_updates)
        apply_updates(priors, prior_updates)
        apply_pipeline_updates(params, pipeline_updates)

        set_output_dir(params, name, output_dir)

    run["params"] = params
    run["values"] = values
    run["priors"] = priors

    return run


def parse_yaml_run_file(run_config):
    """
    Parse a yaml file (or process a previously parsed file)
    containing a list of runs to perform.

    The file should have the following format:

    output_dir: <output directory>

    include: <list of other run files to include>

    components:
        <name of component>:
            params:
            - <list of updates to apply to the parameters file>
            values:
            - <list of updates to apply to the values file>
            priors:
            - <list of updates to apply to the priors file>
            pipeline:
            - <list of updates to apply to the pipeline>
    

    runs:
        - name: <name of the run>
        base: <name of the base parameter file>
        parent: <name of the parent run>
        components:
        - <list of components to include from the list above>
        env:
        <dictionary of environment variables to set>
        params:
        - <list of updates to apply to the parameters file>
        values:
        - <list of updates to apply to the values file>
        priors:
        - <list of updates to apply to the priors file>
        pipeline:
        - <list of updates to apply to the pipeline>
            
                
    Parameters
    ----------
    run_config : str or dict
        The name of the file to parse, or a previously loaded configuration

    Returns
    -------
    runs : dict
        A dictionary of runs, keyed by name
    """
    if isinstance(run_config, dict):
        info = run_config
    else:
        with open(run_config, 'r') as f:
            info = yaml.safe_load(f)
    
    output_dir = info.get("output_dir", ".")

    include = info.get("include", [])
    if isinstance(include, str):
        include = [include]

    # Can include another run file, which we deal with
    # recursively.  
    runs = {}
    for include_file in include:
        runs.update(parse_yaml_run_file(include_file))

    # But we override the output directory
    # of any imported runs with the one we have here   
    for name, run in runs.items():
        set_output_dir(run["params"], name, output_dir)
    
    # deal with re-usable components
    components = info.get("components", {})

    # Build the parameter, value, and prior objects for this run
    for run_dict in info["runs"]:
        name = run_dict["name"]
        runs[name] = build_run(name, run_dict, runs, components, output_dir)

    return runs


def show_run(run):
    """
    Print a complete description of the run to the screen

    Parameters
    ----------
    run : dict
        The run to print
    
    Returns
    -------
    None
    """
    print(underline(f"Run {run['name']}", '='))
    print(underline("Parameters"))
    run["params"].write(sys.stdout)
    print("")
    print(underline("Values"))
    run["values"].write(sys.stdout)
    print("")
    print(underline("Priors"))
    run["priors"].write(sys.stdout)
    print("")

def perform_test_run(run):
    """
    Launch a run under the "test" sampler, which just runs the pipeline
    and does not do any sampling.

    Parameters
    ----------
    run : dict
        The run to perform
    
    Returns
    -------
    status: int
        The exit status of the run
    """
    params = run["params"]
    values = run["values"]
    priors = run["priors"]
    env = run["env"]
    params.set("runtime", "sampler", "test")
    params.set("pipeline", "debug", "T")
    params.set("runtime", "verbosity", "debug")
    params.set("runtime", "resume", "F")

    with temporary_environment(env):
        return run_cosmosis(params, values=values, priors=priors)

def chain_status(filename, include_comments=False):
    n = 0
    line = ""
    for line in open(filename):
        if include_comments or not line.startswith("#"):
            n += 1
    # if the last line
    complete = line.startswith("#complete=1")
    last_update_time = os.path.getmtime(filename)
    time_ago_seconds = time.time() - last_update_time
    time_ago_minutes = time_ago_seconds / 60
    return n, complete, time_ago_minutes

def show_run_status(runs, names=None):
    """
    Report the status of the output of one or more runs.

    Parameters
    ----------
    runs : dict
        A dictionary of runs, keyed by name
    names : list
        The names of the runs to report on.  If None then all runs are reported on.
    
    Returns
    -------
    None
    """
    if not names:
        names = runs.keys()

    for name in names:
        run = runs[name]
        sampler = run["params"].get("runtime", "sampler")
        if sampler == "test":
            output_dir = run["params"].get("test", "save_dir")
            if os.path.exists(output_dir):
                print(f"🟢 {name} has been run [test sampler]")
            else:
                print(f"🔴 {name} has not been run [test sampler]")
        else:
            output_file = run["params"].get("output", "filename")
            if os.path.exists(output_file):
                n, complete, last_update = chain_status(output_file)
                if complete:
                    print(f"🟢 {name} output complete with {n} samples, updated {last_update:.1f} minutes ago")
                elif n:
                    print(f"🟡 {name} output exists with {n} samples, updated {last_update:.1f} minutes ago")
                else:
                    print(f"🟠 {name} output exists with 0 samples, updated {last_update:.1f} minutes ago")
            else:
                print(f"🔴 {name} output missing with 0 samples")


def launch_run(run, mpi=False):
    """
    Launch a CosmoSIS run.

    Parameters
    ----------
    run : dict
        The run to launch
    
    Returns
    -------
    status: int
        The exit status of the run
    """
    params = run["params"]
    values = run["values"]
    priors = run["priors"]
    env = run["env"]

    with temporary_environment(env):
        if mpi:
            with MPIPool() as pool:
                return run_cosmosis(params, values=values, priors=priors, pool=pool)
        else:
            return run_cosmosis(params, values=values, priors=priors)




import argparse
parser = argparse.ArgumentParser(description="Manage and launch CosmoSIS runs")
parser.add_argument("run_config", help="The yaml file containing the runs to perform")
group = parser.add_mutually_exclusive_group(required=True)
group.add_argument("--list", "-l", action="store_true", help="List all available runs")
group.add_argument("--cat", "-c",  type=str, help="Show a single run")
group.add_argument("--status", "-s", default="_unset", nargs="*", help="Show the status of a single run, or all runs if called with no argument")
group.add_argument("--run", "-r",  help="Run the named run")
group.add_argument("--test", "-t",  help="Launch the named run")
parser.add_argument("--mpi", action="store_true", help="Use MPI to launch the runs")



def main(args):
    runs = parse_yaml_run_file(args.run_config)

    if args.mpi and not args.run:
        raise ValueError("MPI can only be used when running a single run")

    status_set = args.status != "_unset"

    # The various command line options, which are mutually exclusive.
    # This is enforced by the parser above.
    if args.list:
        for name in runs:
            print(name)
    elif args.cat:
        show_run(runs[args.cat])
    elif args.test:
        perform_test_run(runs[args.test])
    elif status_set:
        show_run_status(runs, args.status)
    elif args.run:
        launch_run(runs[args.run], mpi=args.mpi)



if __name__ == "__main__":
    args = parser.parse_args()
    main(args)

