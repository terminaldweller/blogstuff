
function dummy_test()
  local dummy = require("dummy")
  local a = a_t()
  local b = b_t()
  local c = c_t()
  local cc = c_t()

  -- checking b_t
  print(b:b_int())
  b:set_b_int(20)
  print(b:b_int())

  print(b:b_float())
  b:set_b_float(33.33)
  print(b:b_float())

  -- checking c_t
  print(c:c_int())
  c:set_c_int(5)
  print(c:c_int())

  print(c:c_string())
  c:set_c_string("hello there...")
  print(c:c_string())

  cc:set_c_int(10)
  cc:set_c_string("hey...")

  local d = {c, cc}

  -- checking a_t
  print("-----------------------------------------------------------")
  print(a:a_int())
  a:set_a_int(5)
  print(a:a_int())

  print(a:a_float())
  a:set_a_float(11.11)
  print(a:a_float())

  print(a:a_string())
  a:set_a_string("hello there...")
  print(a:a_string())

  print(a:a_p())
  a:set_a_p(b)
  print(a:a_p())
  print(a:a_p():b_int())
  print(a:a_p():b_float())

  a:set_a_int(0)
  print(a:a_pp())
  --print("table len:"..#d)
  a:set_a_int(#d)
  a:set_a_pp(d)
  print(a:a_pp())
  for k, v in pairs(a:a_pp()) do
    print(k, v, v:c_string(), v:c_int())
  end

end

dummy_test()
