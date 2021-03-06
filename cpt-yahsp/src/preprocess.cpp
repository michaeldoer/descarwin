/*
 * CPT - a Constraint Programming Temporal planner
 *
 * File : preprocess.c
 *
 * Copyright (C) 2005-2011  Vincent Vidal <Vincent.Vidal@onera.fr>
 */


#include "cpt.h"
#include "options.h"
#include "structs.h"
#include "problem.h"
#include "propagations.h"
#include "preprocess.h"
#include "plan.h"
#include "trace_planner.h"
#include "globs.h"


/*---------------------------------------------------------------------------*/
/* Local Macros                                                              */
/*---------------------------------------------------------------------------*/


#define get_areachable(a) areachable[(a)->id]
#define set_areachable(a, t) areachable[(a)->id] = t
#define get_reached_prec(a) reached_prec[(a)->id]
#define set_reached_prec(a, t) reached_prec[(a)->id] = t
#define inc_reached_prec(a) reached_prec[(a)->id]++
#define get_freachable(a) freachable[(a)->id]
#define set_freachable(a, t) freachable[(a)->id] = t


/*---------------------------------------------------------------------------*/
/* Static Functions                                                          */
/*---------------------------------------------------------------------------*/


static void compute_reachability_a(Action *a, bool areachable[], bool freachable[], size_t reached_prec[]);
static void compute_reachability_f(Fluent *f, bool areachable[], bool freachable[], size_t reached_prec[]);


/*****************************************************************************/


static void compute_reachability_a(Action *a, bool areachable[], bool freachable[], size_t reached_prec[])
{
  if (!get_areachable(a)) {
    set_areachable(a, true);
/*     printf("   %s\n", action_name(a)); */
    FOR(f, a->add) {
      compute_reachability_f(f, areachable, freachable, reached_prec);
    } EFOR;
  }
}

static void compute_reachability_f(Fluent *f, bool areachable[], bool freachable[], size_t reached_prec[])
{
  if (!get_freachable(f)) {
    set_freachable(f, true);
    FOR(a, f->consumers) {
      inc_reached_prec(a);
      if (get_reached_prec(a) == a->prec_nb)
	compute_reachability_a(a, areachable, freachable, reached_prec);
    } EFOR;
  }
}

void compute_bad_supporters(void)
{
  size_t reached_prec[actions_nb];
  bool areachable[actions_nb], freachable[fluents_nb];

  FORCOUPLE(a, actions, f, a->add) {
    FOR(a2, actions) {
      set_areachable(a2, edeletes(a2, f) || produces(a2, f));
      set_reached_prec(a2, 0);
    } EFOR;
    FOR(f, fluents) { set_freachable(f, false); } EFOR;
    FOR(f2, fluents) {
      if (produces(a, f2) || !edeletes(a, f2))
	compute_reachability_f(f2, areachable, freachable, reached_prec);
    } EFOR;
    FOR(c, f->causals) {
      if (can_produce(c, a) && get_reached_prec(c->consumer) < c->consumer->prec_nb) {
	if (opt.verbose_preprocessing) { print_action(a); printf(" cannot support "); print_causal(c); printf("\n"); }
	rem_producer(c, a);
      }
    } EFOR;
  } EFORCOUPLE;
}

void compute_distance_boosting(void)
{
  Action *inter[actions_nb];
  size_t inter_nb = 0, i;
  bool inter_used[actions_nb];

  for (i = 0; i < actions_nb; i++) inter_used[i] = false;
  FORMIN(cons, actions, 2) {
    FOR(c, cons->causals) {
      FORPROD(prod, c) {
	if (!edeletes(prod, c->fluent)) goto end;
	//if (prod->ac_constraints_nb != 0 && prod->ac_constraints[0]->fluent == c->fluent) goto end;
	FOR(f, cons->add) { if (!consumes(prod, f)) goto end; } EFOR;
	FOR(f, prod->add) { if (!edeletes(cons, f)) goto end; } EFOR;
	// added : consumer can delete smth that was true before producer
	if (opt.pddl21) FOR(f, cons->del) { if (!edeletes(prod, f)) goto end; } EFOR;
	inter_nb = 0;
	FORCOUPLE(f, prod->add, a, f->consumers) {
	  if (a->id > 1 && !inter_used[a->id] && !edeletes(a, c->fluent)) {
	    inter[inter_nb++] = a;
	    inter_used[a->id] = true;
	  }
	} EFORCOUPLE;
	if (inter_nb > 0) {
	  TimeVal newdist = MAXCOST;
	  FOR(a, inter) { inter_used[a->id] = false; } EFOR;
	  FORPROD(a2, c) {
	    if (a2->id > 1)
	      inter[inter_nb++] = a2;
	  } EFOR;
	  FOR(a3, inter) {
	    minimize(newdist, distance(prod, a3) + duration(a3) + distance(a3, cons));
	  } EFOR;
	  if (newdist > distance(prod, cons)) {
	    if (opt.verbose_preprocessing) {
	      printf("distance increase : "); print_action(prod); printf("  "); print_causal(c); printf(" ==> "); print_time(cptout, newdist); printf("\n");
	    }
	    set_distance(prod, cons, newdist);
	  }
	} else {
	  if (opt.verbose_preprocessing) {
	    printf("cannot support    : "); print_action(prod); printf("  "); print_causal(c); printf("\n");
	  }
	  rem_producer(c, prod);
	}
      end:;
      } EFOR;
    } EFOR;
  } EFOR;
}


void compute_landmarks(void)
{
  size_t reached_prec[actions_nb + opt.max_plan_length];
  bool areachable[actions_nb + opt.max_plan_length];
  bool freachable[fluents_nb];
  Action *lands[actions_nb + opt.max_plan_length];
  size_t lands_nb = 0;
  
  FORMIN(a2, actions, 2) {
    FOR(f, fluents) { set_freachable(f, false); } EFOR;
    FOR(a, actions) { reached_prec[a->id] = 0; set_areachable(a, false); } EFOR;
    set_areachable(a2, true);
    FOR(a, actions) {
      if (!a->prec_nb) compute_reachability_a(a, areachable, freachable, reached_prec);
    } EFOR;
    FOR(f, end_action->prec) { if (!get_freachable(f)) { lands[lands_nb++] = a2; break; } } EFOR;
  } EFOR;

  propagate();
    
  FOR(a, lands) {
    if (opt.verbose_preprocessing) { print_action(a); printf("\n"); }
    if (!a->used && a->origin->nb_instances > 0) clone_action(a, NULL);
    use_action(a);
    propagate();
  } EFOR;

  FOR(a, active_actions) {
    if (a->id <= 1) continue;
    if (a->origin != a) exit(55);
    FOR(a2, active_actions) {
      if (a2->id <= 1) continue;
      if (a2->origin != a2) exit(55);
      if (a2 != a) {
	FOR(f, fluents) { set_freachable(f, false); } EFOR;
	//FOR(a, actions) { reached_prec[a->num] = 0; set_areachable(a, a != a->origin); } EFOR;
	FOR(a, actions) { reached_prec[a->id] = 0; set_areachable(a, false); } EFOR;
	set_areachable(a2, true);
	FOR(a, actions) { 
	  if (!a->prec) compute_reachability_a(a, areachable, freachable, reached_prec);
	} EFOR;
	if (!get_areachable(a)) goto suite;
	FOR(f, a->add) {
	  if (consumes(end_action, f) && edeletes(a2, f) && !produces(a2, f) && (f->producers_nb == 2 || (f->producers_nb >= 3 && produces(start_action, f)))) {
	    goto suite;
	  }
	} EFOR;
	continue;
      suite:
	if (opt.verbose_preprocessing) { printf("landmark order : "); print_action(a2); printf("  <  "); print_action(a); printf("\n"); }
	order_before_aa(a2, a);
      }
    } EFOR;
  } EFOR;
}

