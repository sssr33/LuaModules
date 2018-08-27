
MyVariable = 123456789

for k,v in pairs(_G) do
    print(k .. ' = ' .. tostring(v))
end

print(k)

do
local MyVariable2 = {}

end

print(tostring(MyVariable2))








local t = _G

t[1] = 3
t[2] = 4
t[3] = 5

--t = {asd = 111 , [0] = 123, 3, 4, 5, 123,}

print('t has ' .. #t .. ' elements:')

for i = 1, #t do
    io.write(t[i] .. '; ')
end
print('\n')

for k,v in pairs(t) do
    print(k .. ' = ' .. tostring(v))
end