local utils = {}

-- https://stackoverflow.com/questions/1283388/how-to-merge-two-tables-overwriting-the-elements-which-are-in-both
function utils.TableMerge(t1, t2)
  for k,v in pairs(t2) do
      if type(v) == "table" then
          if type(t1[k] or false) == "table" then
              tableMerge(t1[k] or {}, t2[k] or {})
          else
              t1[k] = v
          end
      else
          t1[k] = v
      end
  end
  return t1
end


function utils.EnsureFileExists(name)
    local fileCreated = false
    local f = io.open(name,"r")
    if not f then 
        f = io.open(name,"w")
        f:write("{}")
        fileCreated = true
    end
    io.close(f) 
    return fileCreated
end

return utils