require ("lib/utils")

local env = ImportScript("env_head.lua")
LNpcOpject = env.LNpcOpject;
print(111, env, LNpcOpject and LNpcOpject.Name)
for k, v in pairs(env or {}) do
    print(k, v)
end
-- LNpcOpject = LNpcOpject;

LNpcOpject.Name = "npc_1"