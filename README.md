# Introduction
VMTHooking is about replacing the virtual functions inside our class. The reason this is possible is because of the rtti table, it allows us to call an function through rtti table like this rtti[0]. We hook the rtti table and then replace the rtti[0] and then call the original function, the reason the original function is being called is to ensure the flow is not distrubed. Here is an overview of how an vmt hook works.

Replace RTTI Table function with an custom function -> execute our custom function -> call the original function.

That is the method that was used in this project... Once the executable program gets dll injected it will change the string outout...
