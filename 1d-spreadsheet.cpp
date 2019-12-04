#include <iostream>
#include <string>
#include <vector>

struct cell
{
  std::string op;
  std::string lhs;
  std::string rhs;
};
  
int main()
{
  int n;
  std::cin >> n;

  std::vector<cell> cells(n);
  
  for (cell& c : cells)
    std::cin >> c.op >> c.lhs >> c.rhs;

  std::vector<int> queue;
  queue.reserve(n);

  std::vector<int> eval_queue;
  eval_queue.reserve(n);
  
  std::vector<bool> in_queue(n, false);
  
  for (int i(0); i!=n; ++i)
    {
      if (in_queue[i])
        continue;

      queue.emplace_back(i);
      in_queue[i] = true;

      while (!queue.empty())
        {
          const int index(queue.back());
          const cell& cell(cells[index]);
          bool ready(true);

          if (cell.lhs[0] == '$')
            {
              const int ref(std::stoi(cell.lhs.substr(1)));

              if (!in_queue[ref])
                {
                  ready = false;
                  queue.emplace_back(ref);
                  in_queue[ref] = true;
                }
            }

          if (cell.rhs[0] == '$')
            {
              const int ref(std::stoi(cell.rhs.substr(1)));

              if (!in_queue[ref])
                {
                  ready = false;
                  queue.emplace_back(ref);
                  in_queue[ref] = true;
                }
            }

          if (ready)
            {
              queue.pop_back();
              eval_queue.emplace_back(index);
            }
        }
    }

  std::vector<long> result(n);
  
  auto value_of =
    [&](const std::string& arg) -> long
    {
      if (arg[0] == '$')
        {
          const int ref(std::stoi(arg.substr(1)));
          return result[ref];
        }
      else
        return std::stol(arg);
    };

  for (int i : eval_queue)
    {
      const cell& cell(cells[i]);

      if (cell.op == "VALUE")
        result[i] = value_of(cell.lhs);
      else if (cell.op == "ADD")
        result[i] = value_of(cell.lhs) + value_of(cell.rhs);
      else if (cell.op == "SUB")
        result[i] = value_of(cell.lhs) - value_of(cell.rhs);
      else if (cell.op == "MULT")
        result[i] = value_of(cell.lhs) * value_of(cell.rhs);
    }

  for (long r : result)
    std::cout << r << '\n';

  return 0;
}
