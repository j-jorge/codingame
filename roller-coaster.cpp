#include <iostream>
#include <vector>
#include <cassert>

struct result_type
{
  std::size_t value;
  std::size_t current_run;
  std::size_t next_group;
};
  
result_type solve
( const std::vector< std::size_t >& groups, const std::size_t max_capacity,
  const std::size_t runs, std::size_t next_group )
{
  const std::size_t group_count( groups.size() );

  std::size_t result( 0 );

  std::vector< std::size_t > sum_by_start( group_count, 0 );
  std::size_t current_run;
  
  for ( current_run = 0;
        ( current_run != runs ) && ( sum_by_start[ next_group ] == 0 );
        ++current_run )
    {
      std::size_t capacity( max_capacity );
      const std::size_t start( next_group );
      std::size_t run_result( 0 );
      
      while ( capacity >= groups[ next_group ] )
        {
          const std::size_t group_size( groups[ next_group ] );
          run_result += group_size;
          capacity -= group_size;
          next_group = ( next_group + 1 ) % group_count;

          if ( next_group == start )
            break;
        }
      
      result += run_result;
      sum_by_start[ start ] = run_result;
    }

  return { result, current_run, next_group };
}

int main()
{
  std::size_t max_capacity;
  std::size_t runs;
  std::size_t group_count;
  std::cin >> max_capacity >> runs >> group_count;
  
  std::vector< std::size_t > groups( group_count );

  for ( std::size_t& g : groups )
      std::cin >> g;

#if 0
  result_type result( solve( groups, max_capacity, runs, 0 ) );

  std::cout << "initial runs: " << result.current_run << '\n';
  std::size_t i( 0 );
  for ( runs -= result.current_run; runs != 0; )
    {
      ++i;
      result_type r( solve( groups, max_capacity, runs, result.next_group ) );
      result.value += r.value;
      result.next_group = r.next_group;
      std::cout << r.current_run << '\n';
      runs -= r.current_run;
    }
  std::cout << "loops -> " << i << '\n';

  std::cout << result.value << '\n';
#else
  std::size_t result( 0 );
  
  const result_type first( solve( groups, max_capacity, runs, 0 ) );
  result = first.value;
  runs -= first.current_run;

  if ( runs != 0 )
    {
      const result_type repeat
        ( solve( groups, max_capacity, runs, first.next_group ) );
      result += ( runs / repeat.current_run ) * repeat.value;
  
      const result_type remaining
        ( solve
          ( groups, max_capacity, runs % repeat.current_run,
            repeat.next_group ) );
      result += remaining.value;
    }
  
  std::cout << result << '\n';
#endif

  return 0;
}
