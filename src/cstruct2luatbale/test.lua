
function dummy_test()
  local dummy = require("dummy")
  local a = a_t()
  local b = b_t()
  local c = c_t()

  print(a:a_int())
  a:set_a_int(5)
  print(a:a_int())
end

dummy_test()
