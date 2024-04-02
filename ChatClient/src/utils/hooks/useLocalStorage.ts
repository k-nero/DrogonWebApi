import { useState } from "react";

// Simplified useLocalStorage hook
export default function useLocalStorage(key : string, initialValue ) {
    const [storedValue, setStoredValue] = useState(() => {
        try
        {
            const item = window.localStorage.getItem(key);
            return item ? JSON.parse(item) : initialValue;
        }
        catch (error)
        {
            console.log(error);
            return initialValue;
        }
    });

    const setValue = (value) => {
        try
        {
            const valueToStore =  value instanceof Function ? value(storedValue) : value;
            setStoredValue(valueToStore);
            window.localStorage.setItem(key, JSON.stringify(valueToStore));
        }
        catch (error)
        {
            console.log(error);
        }
    };

    return [storedValue, setValue];
}
