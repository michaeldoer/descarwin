 
#ifndef _MAKE_CONTINUE_DAE_H_
#define _MAKE_CONTINUE_DAE_H_


#include <eoCombinedContinue.h>
#include <eoGenContinue.h>
#include <eoSteadyFitContinue.h>
#include <eoEvalContinue.h> 
#include <eoFitContinue.h>
#include <utils/eoParser.h>
#include <utils/eoState.h>
#include <utils/eoFeasibleRatioStat.h>

#include <daex.h>
//#include <cli.h>

namespace daex {

void do_make_continue_param( eoParser & parser )
{
    unsigned int mingen = parser.createParam( (unsigned int)10, "gen-min", 
            "Minimum number of iterations", 'n', "Stopping criterions" ).value();
        eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "mingen" << mingen << std::endl;

    unsigned int steadygen = parser.createParam( (unsigned int)50, "gen-steady", 
            "Number of iterations without improvement", 's', "Stopping criterions" ).value();
        eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "steadygen" << steadygen << std::endl;

    unsigned int maxgens = parser.createParam( (unsigned int)1000, "gen-max", 
            "Maximum number of iterations", 'x', "Stopping criterions" ).value();
        eo::log << eo::logging << FORMAT_LEFT_FILL_W_PARAM << "maxgens" << maxgens << std::endl;
}


template <class EOT>
eoCombinedContinue<EOT> & do_make_continue_op( eoParser & parser, eoState & state )
{
    unsigned int mingen = parser.valueOf<unsigned int>("gen-min");
    unsigned int steadygen = parser.valueOf<unsigned int>("gen-steady");
    unsigned int maxgens = parser.valueOf<unsigned int>("gen-max");

    eoSteadyFitContinue<EOT>* steadyfit = new eoSteadyFitContinue<EOT>( mingen, steadygen );
    state.storeFunctor( steadyfit );

    eoGenContinue<EOT>* maxgen = new eoGenContinue<EOT>( maxgens );
    state.storeFunctor( maxgen );

    // combine the continuators
    eoCombinedContinue<EOT>* continuator = new eoCombinedContinue<EOT>( *steadyfit );
    state.storeFunctor( continuator );

    continuator->add(*maxgen);

    return *continuator;
}


template <class EOT>
eoContinue<EOT> & do_make_continue( eoParser & parser, eoState & state )
{
    do_make_continue_param( parser );
    return do_make_continue_op<EOT>( parser, state );
}



} // namespace daex

#endif // _MAKE_CONTINUE_DAE_H_