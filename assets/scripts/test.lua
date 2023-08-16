--This is a lua test script

Basic_variable = 7 * 6

--lua table test
config = {
    title = "My Game",
    fullscreen = false,
    resolution = {
        width = 800,
        height = 600
    }
}

function Testfunc(n)
    n = n * 2
    print(n);
end

Testfunc(5);

print("Native Function Test "..cube(5));