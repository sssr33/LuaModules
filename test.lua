
mt = {}

function mt:a(arg)
    print('A from metatable')
    print(self)
    print(arg)
end

mt.__index = mt;
mt.__gc = function()
    print('Death')
end

do
    local obj = {}
    setmetatable(obj, mt)
    obj:a(123)
end

collectgarbage('collect')

print('Program end')