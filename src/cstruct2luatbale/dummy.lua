
dummy = {}
setmetatable(a_t, {__call =
	function(self, arg0, arg1, arg2, arg3, arg4)
    if not arg4 then
      arg4 = nil
    end
		local t = self.new(arg0, arg1, arg2, arg3, arg4)
		return t
	end
	}
)

setmetatable(b_t, {__call =
	function(self, arg0, arg1)
		local t = self.new(arg0, arg1)
		return t
	end
	}
)

setmetatable(c_t, {__call =
	function(self, arg0, arg1)
		local t = self.new(arg0, arg1)
		return t
	end
	}
)
return dummy

