/********************************************************************
 * AUTHORS: Arijit Shaw
 *
 * BEGIN DATE: November, 2023
 *
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
********************************************************************/

#include "stp/Sat/UniSamp.h"
#include "approxmc/approxmc.h"
#include "unigen/unigen.h"
#include <unordered_set>
#include <algorithm>
using std::vector;

using namespace CMSat;
using namespace UniGen; // namespace in UniGen library


namespace stp
{

void UniSamp::enableRefinement(const bool enable)
{
  // might break if we simplify with refinement enabled..
  //   if (enable)
  //   {
  //     s->set_no_simplify_at_startup();
  //   }
}

UniSamp::UniSamp()
{
  a = new ApproxMC::AppMC;
  s = new UniG(a);
  // s->log_to_file("stp.cnf");
  //s->set_num_threads(num_threads);
  //s->set_default_polarity(false);
  //s->set_allow_otf_gauss();
  temp_cl = (void*)new vector<CMSat::Lit>;
}

UniSamp::~UniSamp()
{
  delete s;
  vector<CMSat::Lit>* real_temp_cl = (vector<CMSat::Lit>*)temp_cl;
  delete real_temp_cl;
}

void UniSamp::setMaxConflicts(int64_t _max_confl)
{
  max_confl = _max_confl;
}

void UniSamp::setMaxTime(int64_t _max_time)
{
  max_time = _max_time;
}

bool UniSamp::addClause(
    const vec_literals& ps) // Add a clause to the solver.
{
  // Cryptominisat uses a slightly different vec class.
  // Cryptominisat uses a slightly different Lit class too.

  vector<CMSat::Lit>& real_temp_cl = *(vector<CMSat::Lit>*)temp_cl;
  real_temp_cl.clear();
  for (int i = 0; i < ps.size(); i++)
  {
    real_temp_cl.push_back(CMSat::Lit(var(ps[i]), sign(ps[i])));
  }

  return a->add_clause(real_temp_cl);
}

bool UniSamp::okay()
    const // FALSE means solver is in a conflicting state
{
  //return a->okay();
  return true; //TODO AS: implement well
}

bool UniSamp::solve(bool& timeout_expired) // Search without assumptions.
{


  /*
   * STP uses -1 for a value of "no timeout" -- this means that we only set the
   * timeout _in the SAT solver_ if the value is >= 0. This avoids us
   * accidentally setting a large limit (or one in the past).
   */


  // CMSat::lbool ret = s->solve(); // TODO AS
  auto sol_count = a->count();
  s->sample(&sol_count,1);
  CMSat::lbool ret = CMSat::l_True;
  return ret == CMSat::l_True;
}

uint8_t UniSamp::modelValue(uint32_t x) const
{
  // return (s->get_model().at(x) == CMSat::l_True); TODO AS
  return true;
}

uint32_t UniSamp::newVar()
{
  a->new_var();
  return a->nVars() - 1;
}

void UniSamp::setVerbosity(int v)
{
  s->set_verbosity(v);
}

unsigned long UniSamp::nVars() const
{
  return a->nVars();
}

void UniSamp::printStats() const
{
  // s->printStats();
}

void UniSamp::solveAndDump()
  {
     bool t;
     solve(t);
     //s->open_file_and_dump_irred_clauses("clauses.txt");
  }



// Count how many literals/bits get fixed subject to the assumptions..
uint32_t UniSamp::getFixedCountWithAssumptions(const stp::SATSolver::vec_literals& assumps, const std::unordered_set<unsigned>& literals )
{
  /* TODO AS skip all?
  const uint64_t conf = 0; // TODO AS: s->get_sum_conflicts();
  assert(conf == 0);


  // const CMSat::lbool r = s->simplify();  TODO AS

   
  // Add the assumptions are clauses.
  vector<CMSat::Lit>& real_temp_cl = *(vector<CMSat::Lit>*)temp_cl;
  for (int i = 0; i < assumps.size(); i++)
  {
    real_temp_cl.clear();
    real_temp_cl.push_back(CMSat::Lit(var(assumps[i]), sign(assumps[i])));
    a->add_clause(real_temp_cl);
  }


  //std::cerr << assumps.size() << " assumptions" << std::endl;

  std::vector<CMSat::Lit> zero = s->get_zero_assigned_lits();
  for (CMSat::Lit l : zero)
  {
      if (literals.find(l.var()) != literals.end())
        assigned++;
  }
 
 
       
  //std::cerr << assigned << " assignments at end" <<std::endl;

  // The assumptions are each single literals (corresponding to bits) that are true/false. 
  // so in the result they should be all be set
  assert(assumps.size() >= 0);
  assert(assigned >= static_cast<uint32_t>(assumps.size()));
  assert(s->get_sum_conflicts() == conf ); // no searching, so no conflicts.
  assert(CMSat::l_False != r); // always satisfiable.
  */

  uint32_t assigned = 0;

  return assigned;
}



} //end namespace stp
