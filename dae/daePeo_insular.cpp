//
// C++ Implementation: daePeo_insular
//
// Description: 
//
//
// Author:  <>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <sys/time.h>
#include <sys/resource.h>


#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <algorithm>
#include <cstring>
#include <cfloat>

#include <eo>
#include <peo>
#include <do/make_pop.h>
#include <do/make_checkpoint.h>
#include <ga.h>
#include <utils/eoParserLogger.h>
#include <utils/eoLogger.h>
#include <utils/eoFeasibleRatioStat.h>
#include "pack/daePackUnpack.h"
#include "daex.h"
#include "evaluation/yahsp.h"
#include "pack/eoDummyEval.h"

#define LOG_LOCATION(level) eo::log << level << "in " << __FILE__ << ":" << __LINE__ << " " << __PRETTY_FUNCTION__ << std::endl; 

#define LOG_FILL ' '
#define FORMAT_LEFT_FILL_WIDTH(width) "\t" << std::left << std::setfill(LOG_FILL) << std::setw(width) 
#define FORMAT_LEFT_FILL_W_PARAM FORMAT_LEFT_FILL_WIDTH(20)

void print_results( eoPop<daex::Decomposition> pop)
{


    struct rusage usage;
    getrusage(RUSAGE_SELF,&usage);

    eo::log << eo::warnings << "OS statistics:" << std::endl;
    eo::log << eo::warnings << FORMAT_LEFT_FILL_WIDTH(30) << "user time used"               << usage.ru_utime.tv_sec << "." << usage.ru_utime.tv_usec << " s." << std::endl;
    eo::log << eo::warnings << FORMAT_LEFT_FILL_WIDTH(30) << "system time used"             << usage.ru_stime.tv_sec << "." << usage.ru_stime.tv_usec << " s." << std::endl;
    eo::log << eo::warnings << FORMAT_LEFT_FILL_WIDTH(30) << "page reclaims * page size"   << usage.ru_minflt * getpagesize() << " b." 
                                         << " (~ " << std::setprecision(2) << usage.ru_minflt * getpagesize() / 1048576 << " Mb.)" << std::endl; 
    // 1 giga-byte = 1 073 741 824 bytes
    // 1 mega-byte = 1 048 576 octets 
    eo::log << eo::warnings << FORMAT_LEFT_FILL_WIDTH(30) << "page reclaims"                << usage.ru_minflt               << std::endl;
    eo::log << eo::warnings << std::endl;
    eo::log << eo::warnings << FORMAT_LEFT_FILL_WIDTH(30) << "page faults"                  << usage.ru_majflt               << std::endl;
    eo::log << eo::warnings << FORMAT_LEFT_FILL_WIDTH(30) << "maximum resident set size"    << usage.ru_maxrss               << " b." << std::endl;
    eo::log << eo::warnings << FORMAT_LEFT_FILL_WIDTH(30) << "integral shared memory size"  << usage.ru_ixrss                << " b." << std::endl;
    eo::log << eo::warnings << FORMAT_LEFT_FILL_WIDTH(30) << "integral unshared data size"  << usage.ru_idrss                << " b." << std::endl;
    eo::log << eo::warnings << FORMAT_LEFT_FILL_WIDTH(30) << "integral unshared stack size" << usage.ru_isrss                << " b." << std::endl;
    eo::log << eo::warnings << FORMAT_LEFT_FILL_WIDTH(30) << "swaps"                        << usage.ru_nswap                << std::endl;
    eo::log << eo::warnings << FORMAT_LEFT_FILL_WIDTH(30) << "block input operations"       << usage.ru_inblock              << std::endl;
    eo::log << eo::warnings << FORMAT_LEFT_FILL_WIDTH(30) << "block output operations"      << usage.ru_oublock              << std::endl;
    eo::log << eo::warnings << FORMAT_LEFT_FILL_WIDTH(30) << "messages sent"                << usage.ru_msgsnd               << std::endl;
    eo::log << eo::warnings << FORMAT_LEFT_FILL_WIDTH(30) << "messages received"            << usage.ru_msgrcv               << std::endl;
    eo::log << eo::warnings << FORMAT_LEFT_FILL_WIDTH(30) << "signals received"             << usage.ru_nsignals             << std::endl;
    eo::log << eo::warnings << FORMAT_LEFT_FILL_WIDTH(30) << "voluntary context switches"   << usage.ru_nvcsw                << std::endl;
    eo::log << eo::warnings << FORMAT_LEFT_FILL_WIDTH(30) << "involuntary context switches" << usage.ru_nivcsw               << std::endl;

    // the pop being unsorted, sorting it before getting the first is more efficient
    // than using best_element (that uses std::max_element)
    pop.sort();

    std::cout << std::endl << pop.front() << std::endl;
    std::cout << std::endl << pop.front().plan() << std::endl;

    //double subsolver_time = 0;
    unsigned int subsolver_steps = 0;
    for( unsigned int i=0; i < pop.size(); i++ ) {
        for( unsigned int j=0; j < pop[i].subplans().size(); j++ ) {
             //subsolver_time += pop[i].subplan(j).time_subsolver();
             subsolver_steps += pop[i].subplan(j).search_steps();
        }
    }
    eo::log << eo::progress << "; DAEx sub-solver steps " << subsolver_steps << std::endl;
    //eo::log << eo::progress << "; DAEx sub-solver time " << subsolver_time << " s (u-CPU)" << std::endl;

    eo::log << eo::progress << "; DAEx whole time " << usage.ru_utime.tv_sec << "." << usage.ru_utime.tv_usec << " (seconds of user time in CPU)" << std::endl;
}



int main ( int argc, char* argv[] )
{
    peo :: init( argc, argv );   
     
    rng.reseed(time(0));

 
    struct rlimit limit;
    getrlimit(RLIMIT_AS, &limit);
    eo::log << eo::logging << "Maximum size of the process virtual memory (soft,hard)=" << limit.rlim_cur << ", " << limit.rlim_max << std::endl;
    getrlimit(RLIMIT_DATA, &limit);
    eo::log << eo::logging << "Maximum size of the process   data segment (soft,hard)=" << limit.rlim_cur << ", " << limit.rlim_max << std::endl;

    /*
    limit.rlim_cur=100000000;
    limit.rlim_max=100000000;
    setrlimit(RLIMIT_AS, &limit);
    setrlimit(RLIMIT_DATA, &limit);

    getrlimit(RLIMIT_AS, &limit);
    std::cout << "Maximum size of the process virtual memory (soft,hard)=" << limit.rlim_cur << ", " << limit.rlim_max << std::endl;
    getrlimit(RLIMIT_DATA, &limit);
    std::cout << "Maximum size of the process   data segment (soft,hard)=" << limit.rlim_cur << ", " << limit.rlim_max << std::endl;
    */


    // EO

    eoParserLogger parser(argc, argv);
    make_verbose(parser);

    eoState state;

    eo::log << eo::logging << "Parameters:" << std::endl;

    // createParam (ValueType _defaultValue, std::string _longName, std::string _description, char _shortHand=0, std::string _section="", bool _required=false)
    std::string domain = parser.createParam( (std::string)"domain-zeno-time.pddl", "domain", "PDDL domain file", 'D', "Problem", true ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "domain" << domain << std::endl;

    std::string instance = parser.createParam( (std::string)"zeno10.pddl", "instance", "PDDL instance file", 'I', "Problem", true ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "instance" << instance << std::endl;

    bool is_sequential = parser.createParam( (bool)false, "sequential", "Is the problem a sequential one?", 'q', "Problem", true ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "is_sequential" << is_sequential << std::endl;


    // un total de 20 paramètres

    // Initialization
    unsigned int pop_size = parser.createParam( (unsigned int)100, "popSize", "Population Size", 'P', "Evolution Engine").value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "pop_size" << pop_size << std::endl;
    

    unsigned int seed = parser.createParam( (unsigned int)0, "seed", "Random number seed", 'S' ).value();
    if ( seed == 0) {
        seed = time(0);
    }
    rng.reseed( seed );
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "seed" << seed << std::endl;


    unsigned int l_max_init_coef = parser.createParam( (unsigned int)2, "lmax-initcoef", 
            "l_max will be set to the size of the chrono partition * this coefficient", 'C', "Initialization" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "l_max_init_coef" << l_max_init_coef << std::endl;


    unsigned int l_min = parser.createParam( (unsigned int)1, "lmin", 
            "Minimum number of goals in a decomposition", 'l', "Initialization" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "l_min" << l_min << std::endl;


    // Selection
    unsigned int toursize = parser.createParam( (unsigned int)5, "tournament", 
            "Size of the deterministic tournament for the selection", 't', "Selection" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "toursize" << toursize << std::endl;


    double offsprings = parser.createParam( (double)700, "offsprings", 
            "Number of offsprings to produces", 'f', "Selection" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "offsprings" << offsprings << std::endl;


    // Evaluation
    unsigned int fitness_weight = parser.createParam( (unsigned int)10, "fitness-weight", 
            "Unknown weight in the feasible and unfeasible fitness computation", 'W', "Evaluation" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "fitness_weight" << fitness_weight << std::endl;

    unsigned int fitness_penalty = parser.createParam( (unsigned int)2, "fitness-penalty", 
            "Penalty in the unfeasible fitnesses computation", 'w', "Evaluation" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "fitness_penalty" << fitness_penalty << std::endl;


    unsigned int b_max_init = parser.createParam( (unsigned int)1e4, "bmax-init", 
            "Number of allowed expanded nodes for the initial computation of b_max", 'B', "Evaluation" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "b_max_init" << b_max_init << std::endl;

    double b_max_quantile = parser.createParam( (double)0.5, "bmax-quantile", 
            "Quantile to use for estimating b_max (in [0,1], 0.5=median)", 'Q', "Evaluation" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "b_max_quantile" << b_max_quantile << std::endl;
    if( b_max_quantile < 0 || b_max_quantile > 1 ) {
        std::cout << "bmax-quantile must be a double in [0,1] (=" << b_max_quantile << ") type --help for usage." << std::endl;
        exit(1);
    }

    unsigned int b_max_fixed = parser.createParam( (unsigned int)0, "bmax-fixed", 
            "Fixed number of allowed expanded nodes. Overrides bmaxinit if != 0", 'b', "Evaluation" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "b_max_fixed" << b_max_fixed << std::endl;


    double b_max_last_weight = parser.createParam( (double)3, "bmax-last-weight",
            "Weighting for the b_max used during the last search towards the end goal (must be strictly positive)", 'T', "Evaluation" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "b_max_last_weight" << b_max_last_weight << std::endl;

    if( b_max_last_weight <= 0 ) {
        std::cout << "bmax-last-weight must be strictly positive (=" << b_max_last_weight << ") type --help for usage." << std::endl;
        exit(1);
    }


    // Other
    unsigned int maxtry_candidate = 0; // deactivated by default: should try every candidates
    unsigned int maxtry_mutex = 0;     // deactivated by default: should try every candidates
    /*
    unsigned int maxtry_candidate = parser.createParam( (unsigned int)11, "maxtry-candidate", 
            "Maximum number of atoms to try when searching for a candidate in the changeAtom mutation", 'y', "Misc" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "maxtry_candidate" << maxtry_candidate << std::endl;


    unsigned int maxtry_mutex = parser.createParam( (unsigned int)11, "maxtry-mutex", 
            "Maximum number of atoms to try when searching for mutexes in the changeAtom mutation", 'z', "Misc" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "maxtry_mutex" << maxtry_mutex << std::endl;
    */

    std::string plan_file = parser.createParam( (std::string)"plan.ipc", "plan-file", "Plan file backup", '1', "Misc" ).value();
    
    std::string plan_file2 = parser.createParam( (std::string)"plan2.ipc", "plan-file2", "Plan file backup", '2', "Misc" ).value();
    
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "plan-file" << plan_file << std::endl;
    
    
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "plan-file2" << plan_file2 << std::endl;

    // Variation
    unsigned int radius = parser.createParam( (unsigned int)2, "radius", 
            "Number of neighbour goals to consider for the addGoal mutation", 'R', "Variation" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "radius" << radius << std::endl;

    double proba_change = parser.createParam( (double)0.8, "proba-change", 
            "Probability to change an atom for the changeAtom mutation", 'c', "Variation" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "proba_change" << proba_change << std::endl;

    double proba_del_atom = parser.createParam( (double)0.8, "proba-del-atom", 
            "Average probability to delete an atom for the delAtom mutation", 'd', "Variation" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "proba_change" << proba_change << std::endl;

    double w_delgoal = parser.createParam( (double)1, "w-delgoal", 
            "Relative weight defining the probability to call the delGoal mutation", 'a', "Variation" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "w_delgoal" << w_delgoal << std::endl;


    double w_addgoal = parser.createParam( (double)3, "w-addgoal", 
            "Relative weight defining the probability to call the addGoal mutation", 'A', "Variation" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "w_addgoal" << w_addgoal << std::endl;


    double w_delatom = parser.createParam( (double)1, "w-delatom", 
            "Relative weight defining the probability to call the delAtom mutation", 'g', "Variation" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "w_delatom" << w_delatom << std::endl;


    double w_addatom = parser.createParam( (double)1, "w-addatom", 
            "Relative weight defining the probability to call the addAtom mutation", 'G', "Variation" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "w_addatom" << w_addatom << std::endl;


    double proba_cross = parser.createParam( (double)0.2, "proba-cross", 
            "Probability to apply a cross-over", 'c', "Variation" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "proba_cross" << proba_cross << std::endl;


    double proba_mut = parser.createParam( (double)0.8, "proba-mut", 
            "Probability to apply one of the mutation", 'm', "Variation" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "proba_mut" << proba_mut << std::endl;


    // Stopping criterions
    unsigned int max_seconds = parser.createParam( (unsigned int)10800, "max-seconds", 
            "Maximum number of seconds for the whole search", 'i', "Stopping criterions" ).value(); // 10800 seconds = 30 minutes
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "max_seconds" << max_seconds << std::endl;


    unsigned int mingen = parser.createParam( (unsigned int)10, "gen-min", 
            "Minimum number of iterations", 'n', "Stopping criterions" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "mingen" << mingen << std::endl;


    unsigned int steadygen = parser.createParam( (unsigned int)50, "gen-steady", 
            "Number of iterations without improvement", 's', "Stopping criterions" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "steadygen" << steadygen << std::endl;


    unsigned int maxgens = parser.createParam( (unsigned int)1000, "gen-max", 
            "Maximum number of iterations", 'x', "Stopping criterions" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "maxgens" << maxgens << std::endl;

    unsigned int maxruns = parser.createParam( (unsigned int)1, "runs-max", 
            "Maximum number of runs, if x==0: no limit, if x>1: will do multi-start", 'r', "Stopping criterions" ).value();
    eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "maxruns" << maxruns << std::endl;

     eoValueParam <std :: string> schema_param ("schema.xml", "schema", "ParallelSchema");
 
     parser.processParam (schema_param);

//  eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "schema_param" << schema_param << std::endl;
     
     eoValueParam <std :: string> debug_param ("false", "debug", "?");
 
     parser.processParam (debug_param);
     
     unsigned int migration_freq  = parser.createParam((unsigned int)(2), "migFreq", "Migration frequency",'y',"Param").value();
     
     
      unsigned int nbMigrants = parser.createParam((unsigned int)(1), "migSize", "Migration size",'e',"Param").value();
  
  
    
     make_help( parser );


    /***********
     * PARSING *
     ***********/

    // PDDL

    // parse les pddl
    // FIXME ATTENTION : il y a une otion cachée dans l'init qui précise à yahsp si on est en temporel ou en séquentiel, il faut la régler correctement en fonction du problème visé
    eo::log << eo::progress << "Load the instance..." << std::endl;
    eo::log.flush();
    
    daex::pddlLoad pddl( domain, instance, SOLVER_YAHSP, HEURISTIC_H1, is_sequential );
    
    //daex::pddlLoad pddl2( domain, instance, SOLVER_YAHSP, HEURISTIC_H1, is_sequential );
    
    //daex::pddlLoad pddl2( domain, instance, SOLVER_YAHSP, HEURISTIC_H1, is_sequential );
    
    
    
    
    eo::log << eo::progress << "Load the instance...OK" << std::endl;


    eo::log << eo::progress << "Initialization...";
    eo::log.flush();

    // lie les structures de données DAEx à celles de YAHSP
//    bindDaeYahsp( pddl );

    /******************
     * INITIALIZATION *
     ******************/

    // l'initialisation se fait en fonction de la liste des dates au plus tot possibles (start time set)
    // Note : dans le init, l_max est réglé au double du nombre de dates dans la partition
    daex::Init init( pddl.chronoPartitionAtom(), l_max_init_coef, l_min );
    
    daex::Init init2( pddl.chronoPartitionAtom(), l_max_init_coef, l_min );
  
    // randomly generate the population with the init operator
    eoPop<daex::Decomposition> pop = eoPop<daex::Decomposition>( pop_size, init );
    
    eoPop<daex::Decomposition> pop2 = eoPop<daex::Decomposition>( pop_size, init2 );

    eo::log << eo::progress << "OK" << std::endl;
    

    // b_max for all the searches within the decomposition, between intermediate goals
    unsigned int b_max_in, b_max_in2 ;
    
    

    // if we want to estimate the initial b_max other a first search step
    if( b_max_fixed == 0 ) {
        // FIRST INIT LOOP FOR B_MAX ESTIMATION

        eo::log << eo::progress << "Evaluation of b_max";
        eo::log << eo::logging << " (used node numbers)";
        eo::log << eo::progress << "...";
        eo::log << eo::log.flush();

        // An different evaluator for the first iteration
        // permits to compute the b_max
        // (b_max = 10 000 by default)
        // uses the same value for b_max_in and b_max_last
        daeYahspEvalInit eval_yahsp_init( pop.size(), init.l_max(), b_max_init, b_max_init, fitness_weight, fitness_penalty );
        
        daeYahspEvalInit eval_yahsp_init2( pop2.size(), init2.l_max(), b_max_init, b_max_init, fitness_weight, fitness_penalty );

        // start the eval on the first random pop
        eoPopLoopEval<daex::Decomposition> eval_init( eval_yahsp_init );
        
        eoPopLoopEval<daex::Decomposition> eval_init2( eval_yahsp_init2 );
         
        eval_init( pop, pop );
        
        eval_init2( pop2, pop2 );

        // estimate the b_max from the eval results
         b_max_in = eval_yahsp_init.estimate_b_max( b_max_quantile );
         
          b_max_in2 = eval_yahsp_init2.estimate_b_max( b_max_quantile );

    // if we want a fixed b_max for the whole search
    } else { // if b_max_in != 0
        b_max_in = b_max_fixed;
        eo::log << eo::progress << "No evaluation of b_max, fixed to...";
        eo::log.flush();
    }

    assert( b_max_in > 0 );

    // The b_max for the very last search towards the end goal
    // is the estimated b_max * a given weight (3 by default)
    unsigned int b_max_last = static_cast<unsigned int>( std::floor( b_max_in * b_max_last_weight ) );

    assert( b_max_last > 0 );
 
    eo::log << eo::logging << std::endl << "\tb_max for intermediate goals, b_max_in: "   << b_max_in   << std::endl;
    eo::log << eo::logging              << "\tb_max for        final goal,  b_max_last: " << b_max_last << std::endl;
    eo::log << eo::progress << "OK" << std::endl;

    eo::log << eo::progress << "Creating evaluators...";
    eo::log << eo::log.flush();

    // nested evals:

    // eval that uses the correct b_max
    daeYahspEval eval_yahsp( init.l_max(), b_max_in, b_max_last, fitness_weight, fitness_penalty );
    
    daeYahspEval eval_yahsp2( init2.l_max(), b_max_in2, b_max_last, fitness_weight, fitness_penalty );

    // counter, for checkpointing
    eoEvalFuncCounter<daex::Decomposition> eval_counter( eval_yahsp, "Eval.\t" );
    
    
    eoEvalFuncCounter<daex::Decomposition> eval_counter2( eval_yahsp2, "Eval.\t" );

    // an eval that raises an exception if maxtime is reached
    //eoEvalTimeThrowException<daex::Decomposition> eval_maxtime( eval_counter, max_seconds );
    
    
    //eoEvalTimeThrowException<daex::Decomposition> eval_maxtime2( eval_counter2, max_seconds );
   

    eo::log << eo::progress << "OK" << std::endl;

    // TODO Sachant qu'on les a déjà évalués avec un b_max élevé, on ne veut pas les réévaluer avec un b_max plus petit, donc on ne réévalue pas après l'init
    //eoPopLoopEval<daex::Decomposition> pop_eval( eval_maxtime );
    
    
    //eoPopLoopEval<daex::Decomposition> pop_eval2( eval_maxtime2 );
    
    
   // pop_eval( pop, pop );
    
    //pop_eval2 (pop2,pop2);


    /********************
     * EVOLUTION ENGINE *
     ********************/
   
 
    eo::log << eo::progress << "Algorithm instanciation...";
    eo::log.flush();

    // STOPPING CRITERIA
    
    /*
  ____________________________________________
 | Checkpoint                                 |
 |    __________________________              |
 | <-| Combined Continue        |             |
 |   |    _____________________ |             |
 |   | <-| Steady Fit Continue ||             |
 |   |   |_____________________||             |
 |   |    ______________        |             |
 |   | <-| Gen Continue |       |             |
 |   |   |______________|       |             |
 |   |__________________________|             |
 |                       ___________________  |
 |                   <--| Best fitness stat | |
 |    _________________ |                   | |
 | <-| Stdout monitor  ||                   | |
 |   |               <--|___________________| |
 |   |                 |                      | ______________
 |   |               <-------------------------| Eval Counter |
 |   |                 |                      ||______________|
 |   |_________________|                      |
 |____________________________________________|
    */

    // continuators == stopping criteria
    eoSteadyFitContinue<daex::Decomposition> steadyfit( mingen, steadygen );
    eoGenContinue<daex::Decomposition> maxgen( maxgens );

    // combine the continuators
    eoCombinedContinue<daex::Decomposition> continuator( steadyfit );
    continuator.add(maxgen);

    // attach a continuator to the checkpoint
    // the checkpoint is here to get some stat during the search
    eoCheckPoint<daex::Decomposition> checkpoint( continuator );


    // get the best plan only if it improve the fitness
    // note: fitness is different from the makespan!
    // worst fitness, TODO we should use the best individual from the previous evals at init
    eoMinimizingDualFitness worst_fitness( std::make_pair<double,bool>( DBL_MAX, 1 ) );
    eoBestPlanImprovedStat<daex::Decomposition> best_plan( worst_fitness, "Best improved plan");
    // at each generation
    checkpoint.add( best_plan );
    
    // display the stats on std::cout
    // ostream & out, bool _verbose=true, std::string _delim = "\t", unsigned int _width=20, char _fill=' ' 
    eoOStreamMonitor cout_monitor( std::cout, "\t", 10, ' '); 


    // get best fitness
    // for us, has the form "fitness feasibility" (e.g. "722367 1")
    eoBestFitnessStat<daex::Decomposition> best_stat("Best");

    //eoInterquartileRangeStat<daex::Decomposition> iqr_stat( std::make_pair(0.0,false), "IQR" );
    eoInterquartileRangeStat<daex::Decomposition> iqr_f( std::make_pair(0.0,false), "IQR_f" );
    eoInterquartileRangeStat<daex::Decomposition> iqr_uf( std::make_pair(0.0,false), "IQR_uf" );
    eoDualStatSwitch<daex::Decomposition,eoInterquartileRangeStat<daex::Decomposition> > dual_iqr( iqr_f, iqr_uf, "\t" );
 
    // TODO implement "better" nth_element stats with different interpolations (linear and second moment?)
    eoNthElementFitnessStat<daex::Decomposition> median_stat( pop.size() / 2, "Median" ); 
    /*
    eoNthElementFitnessStat<daex::Decomposition> median_f( pop.size() / 2, "Median_f" ); 
    eoNthElementFitnessStat<daex::Decomposition> median_uf( pop.size() / 2, "Median_uf" ); 
    eoDualStatSwitch<daex::Decomposition,eoNthElementFitnessStat<daex::Decomposition> > dual_median( median_f, median_uf, "\t/\t" );
    */

    eoFeasibleRatioStat<daex::Decomposition> feasible_stat( "F.Ratio" );

    eoAverageSizeStat<daex::Decomposition> asize_stat( "Av.Size" );

    if( eo::log.getLevelSelected() >= eo::progress ) {

        // compute stas at each generation
        checkpoint.add( best_stat );
        checkpoint.add( feasible_stat );
        checkpoint.add( asize_stat );
        checkpoint.add( median_stat );
        //checkpoint.add( dual_median );
        //checkpoint.add( iqr_stat );
        checkpoint.add( dual_iqr );
    
        cout_monitor.add( eval_counter );
        cout_monitor.add( best_stat );
        cout_monitor.add( asize_stat );
        cout_monitor.add( feasible_stat );
        cout_monitor.add( median_stat );
        //cout_monitor.add( dual_median );
        //cout_monitor.add( iqr_stat );
        cout_monitor.add( dual_iqr );
        
        // the checkpoint should call the monitor at every generation
        checkpoint.add( cout_monitor );
    }

    // append the plans in a file
    // std::string _filename, std::string _delim = " ", bool _keep = false, bool _header=false, bool _overwrite = false
    eoFileMonitor file_monitor( plan_file, "\n", false, false, true);
    file_monitor.add( best_plan );

    checkpoint.add( file_monitor );
    
    
    ///Continuator 2
    
    
    eoSteadyFitContinue<daex::Decomposition> steadyfit2( mingen, steadygen );
    eoGenContinue<daex::Decomposition> maxgen2( maxgens );

    // combine the continuators
    eoCombinedContinue<daex::Decomposition> continuator2( steadyfit2 );
    continuator2.add(maxgen2);

    // attach a continuator to the checkpoint
    // the checkpoint is here to get some stat during the search
    eoCheckPoint<daex::Decomposition> checkpoint2( continuator2 );


    // get the best plan only if it improve the fitness
    // note: fitness is different from the makespan!
    // worst fitness, TODO we should use the best individual from the previous evals at init
    eoMinimizingDualFitness worst_fitness2( std::make_pair<double,bool>( DBL_MAX, 1 ) );
    eoBestPlanImprovedStat<daex::Decomposition> best_plan2( worst_fitness2, "Best improved plan");
    // at each generation
    checkpoint2.add( best_plan2 );
    
    // display the stats on std::cout
    // ostream & out, bool _verbose=true, std::string _delim = "\t", unsigned int _width=20, char _fill=' ' 
    eoOStreamMonitor cout_monitor2( std::cout, "\t", 10, ' '); 


    // get best fitness
    // for us, has the form "fitness feasibility" (e.g. "722367 1")
    eoBestFitnessStat<daex::Decomposition> best_stat2("Best");

    //eoInterquartileRangeStat<daex::Decomposition> iqr_stat( std::make_pair(0.0,false), "IQR" );
    eoInterquartileRangeStat<daex::Decomposition> iqr_f2( std::make_pair(0.0,false), "IQR_f" );
    eoInterquartileRangeStat<daex::Decomposition> iqr_uf2( std::make_pair(0.0,false), "IQR_uf" );
    eoDualStatSwitch<daex::Decomposition,eoInterquartileRangeStat<daex::Decomposition> > dual_iqr2( iqr_f2, iqr_uf2, "\t" );
 
    // TODO implement "better" nth_element stats with different interpolations (linear and second moment?)
    eoNthElementFitnessStat<daex::Decomposition> median_stat2( pop2.size() / 2, "Median" ); 
    /*
    eoNthElementFitnessStat<daex::Decomposition> median_f( pop.size() / 2, "Median_f" ); 
    eoNthElementFitnessStat<daex::Decomposition> median_uf( pop.size() / 2, "Median_uf" ); 
    eoDualStatSwitch<daex::Decomposition,eoNthElementFitnessStat<daex::Decomposition> > dual_median( median_f, median_uf, "\t/\t" );
    */

    eoFeasibleRatioStat<daex::Decomposition> feasible_stat2( "F.Ratio" );

    eoAverageSizeStat<daex::Decomposition> asize_stat2( "Av.Size" );

    if( eo::log.getLevelSelected() >= eo::progress ) {

        // compute stas at each generation
        checkpoint2.add( best_stat2 );
        checkpoint2.add( feasible_stat2 );
        checkpoint2.add( asize_stat2 );
        checkpoint2.add( median_stat2 );
        //checkpoint.add( dual_median );
        //checkpoint.add( iqr_stat );
        checkpoint2.add( dual_iqr2 );
    
        cout_monitor2.add( eval_counter2 );
        cout_monitor2.add( best_stat2 );
        cout_monitor2.add( asize_stat2 );
        cout_monitor2.add( feasible_stat2 );
        cout_monitor2.add( median_stat2 );
        //cout_monitor.add( dual_median );
        //cout_monitor.add( iqr_stat );
        cout_monitor2.add( dual_iqr2 );
        
        // the checkpoint should call the monitor at every generation
        checkpoint2.add( cout_monitor2 );
    }

    // append the plans in a file
    // std::string _filename, std::string _delim = " ", bool _keep = false, bool _header=false, bool _overwrite = false
    eoFileMonitor file_monitor2( plan_file2, "\n", false, false, true);
    file_monitor2.add( best_plan2 );

    checkpoint2.add( file_monitor2 );

    
    
    // SELECTION
    // TODO cet opérateur, fait soit un tri de la pop (true), soit un shuffle (false), idéalement, on ne voudrait ni l'un ni l'autre, car on parcours tout, peu importe l'ordre
    
    // JACK the article indicate that tere is no selection, but the article use a deterministic tournament

    // L'article indique qu'il n'y a pas de sélection, on aurait alors ça :
    //eoSequentialSelect<daex::Decomposition> selectone( true );

    /// MAIS le code utilise un tournoi déterministe, on a donc ça :
    eoDetTournamentSelect<daex::Decomposition> selectone( toursize );

    eoDetTournamentSelect<daex::Decomposition> selectone2( toursize );
    // VARIATION

    // mutations
    daex::MutationDelGoal delgoal;
    
    daex::MutationDelGoal delgoal2;
//    daex::MutationDelOneAtom delatom;
    daex::MutationDelAtom delatom( proba_del_atom );
    daex::MutationDelAtom delatom2( proba_del_atom );
    // partition, radius, l_max
    daex::MutationAddGoal addgoal( pddl.chronoPartitionAtom(), radius /*, init.l_max()*/ );
    
    
    daex::MutationAddGoal addgoal2( pddl.chronoPartitionAtom(), radius /*, init.l_max()*/ );
    // partition, proba_change, proba_add, maxtry_search_candidate, maxtry_search_mutex 
    // (maxtry à 0 pour essayer tous les atomes)
    //    daex::MutationAddAtom addatom( pddl.chronoPartitionAtom(), 0.8, 0.5, 11, 11 );
    //    daex::MutationAddAtom addatom( pddl.chronoPartitionAtom(), proba_change, maxtry_candidate, maxtry_mutex );
    daex::MutationChangeAddAtom addatom( pddl.chronoPartitionAtom(), proba_change, maxtry_candidate, maxtry_mutex );
    
    
    daex::MutationChangeAddAtom addatom2( pddl.chronoPartitionAtom(), proba_change, maxtry_candidate, maxtry_mutex );

    // call one of operator, chosen randomly
    eoPropCombinedMonOp<daex::Decomposition> mutator( delgoal, w_delgoal );
    mutator.add( delatom, w_delatom );
    mutator.add( addgoal, w_addgoal );
    mutator.add( addatom, w_addatom );
    
    
    eoPropCombinedMonOp<daex::Decomposition> mutator2( delgoal2, w_delgoal );
    mutator2.add( delatom2, w_delatom );
    mutator2.add( addgoal2, w_addgoal );
    mutator2.add( addatom2, w_addatom );

    // crossover
    // JACK in the crossover, filter out the right half of the decomposition from goals that have a greater date than the date at which we cut
    daex::CrossOverTimeFilterHalf crossover;

    daex::CrossOverTimeFilterHalf crossover2;
    // first call the crossover with the given proba, then call the mutator with the given proba
    // FIXME most of the mutation use the last_reached information, what if they are called after a crossover?
    eoSGAGenOp<daex::Decomposition> variator( crossover, proba_cross, mutator, proba_mut);
    
    
    eoSGAGenOp<daex::Decomposition> variator2( crossover2, proba_cross, mutator2, proba_mut);

    // selector, variator, rate (for selection), interpret_as_rate
    eoGeneralBreeder<daex::Decomposition> breed( selectone, variator, offsprings, false ); 
    
    eoGeneralBreeder<daex::Decomposition> breed2( selectone2, variator2, offsprings, false ); 
    
    // FIXME tester si on veut 700% ou 700


    // REPLACEMENT
    
    // JACK : L'article indique qu'on fait un remplacement en tournoi déterministe et qu'il n'y a pas d'élistisme, on aurait alors ça :
    //eoSSGADetTournamentReplacement<daex::Decomposition> replace(5);
    
    // MAIS le code utilise un remplacement intégral des parents par le meilleur des parents+enfants, avec élitisme faible, on a donc :
    eoCommaReplacement<daex::Decomposition> commareplace;
    
    eoWeakElitistReplacement<daex::Decomposition> replace( commareplace );

	eoCommaReplacement<daex::Decomposition> commareplace2;
	
	 eoWeakElitistReplacement<daex::Decomposition> replace2( commareplace2 );

    
    
   /// Define the topology of your island model
    RingTopology topology;
    
    ///Migration strategy for the island I
    
     eoPeriodicContinue< daex::Decomposition > mig_counter1(  migration_freq );
    
    eoContinuator<daex::Decomposition> mig_cont1(mig_counter1,pop);
    
    eoRandomSelect<daex::Decomposition> mig_select_one;
    
    eoSelector <daex::Decomposition, eoPop<daex::Decomposition> > mig_select1 
    
    (mig_select_one,nbMigrants,pop);
    
    eoPlusReplacement<daex::Decomposition> mreplace1;
    
    eoReplace <daex::Decomposition, eoPop<daex::Decomposition> > mig_replace1 (mreplace1,pop);
    
    peoAsyncIslandMig< eoPop< daex::Decomposition >, eoPop< daex::Decomposition> > migration1 (mig_cont1 ,mig_select1,mig_replace1,topology);
    
    checkpoint.add( migration1 );
  

    /// Migration strategy of the island II
    
    
   eoPeriodicContinue< daex::Decomposition > mig_counter2(  migration_freq );
  
   eoContinuator<daex::Decomposition> mig_cont2(mig_counter2,pop2);
  
  eoRandomSelect<daex::Decomposition> mig_select_two;
  
  eoSelector <daex::Decomposition, eoPop<daex::Decomposition> > mig_select2 (mig_select_two, nbMigrants,pop2);
  
  eoPlusReplacement<daex::Decomposition> mreplace2;
  
  eoReplace <daex::Decomposition, eoPop<daex::Decomposition > > mig_replace2 (mreplace2,pop2);
  
  peoAsyncIslandMig< eoPop< daex::Decomposition >, eoPop< daex::Decomposition > > migration2 ( mig_cont2 ,mig_select2,mig_replace2,topology);
  
  checkpoint2.add( migration2 );
  
  
  /// ALGORITHMS//counter
  
    eoDummyEval plainEval1; 
    
    eoDummyEval plainEval2; 
    
     eoEasyEA<daex::Decomposition> dae( checkpoint,   plainEval1 , breed, replace ); //eval_counter  Ici le test est fait sur un evaluateur factice
      
     eoEasyEA<daex::Decomposition> dae2( checkpoint2,  plainEval2 , breed2, replace2 );//eval_counter
    
  
  ///Definition des Wrappers
  
  
  peoWrapper island1( dae, pop );
  migration1.setOwner( island1 );
  
  peoWrapper island2( dae2, pop2 );
  migration2.setOwner( island2 );
  
  
   peo :: run( );
   
   peo :: finalize( );
     
  
     if (getNodeRank()==1)
     {
  	 std::cout <<   ">>>>>>>>>>>>>>>>>>>>>>>>>>Final best1 :\n" <<  std::endl;
     
         daex::Decomposition best1 = pop.best_element();
   	  
   	 best1.printOn(std::cout);
 	   
 
     }

  
   
 if (getNodeRank()==2)
     {
 	std::cout <<   ">>>>>>>>>>>>>>>>>>>>>>>>>>Final best2 :\n" <<  std::endl;
 	
 	   daex::Decomposition best2 = pop2.best_element();
 	  
 	   best2.printOn(std::cout);
 
 
     }

    return 0;
}
 
  
 /*  try { 

    //    while( 1 ) {

            eo::log << eo::progress << "Start the " << run << "th run..." << std::endl;

            // start a search
            dae( pop );

            // remember the best of all runs
            daex::Decomposition best_of_run = pop.best_element();

            // note: operator> is overloaded in EO, don't be afraid: we are minimizing
            if( best_of_run.fitness() > best.fitness() ) { 
               best = best_of_run;
            }

            // TODO handle the case when we have several best decomposition with the same fitness but different plans?
            // TODO use previous searches to re-estimate a better b_max?

            // the loop test is here, because if we've reached the number of runs, we do not want to redraw a new pop
            run++;
            if( run >= maxruns && maxruns != 0 ) {
                break;
            }

            // Once the bmax is known, there is no need to re-estimate it,
            // thus we re-init ater the first search, because the pop has already been created before,
            // when we were trying to estimate the b_max.
            pop = eoPop<daex::Decomposition>( pop_size, init );
            
            // evaluate
            eoPopLoopEval<daex::Decomposition> pop_eval( eval_maxtime );
            pop_eval( pop, pop );

            // reset run's continuator counters
            steadyfit.totalGenerations( mingen, steadygen );
            maxgen.totalGenerations( maxgens );
        }


    } catch( std::exception& e ) {
        eo::log << eo::warnings << "STOP: " << e.what() << std::endl;
        eo::log << eo::progress << "... premature end of search, current result:" << std::endl;

        // push the best result, in case it was not in the last run
        pop.push_back( best );
        print_results( pop );
        return 0;
    }

    eo::log << eo::progress << "... end of search" << std::endl;

    // push the best result, in case it was not in the last run
    pop.push_back( best );
    print_results( pop );

    return 0;
}
*/

 
