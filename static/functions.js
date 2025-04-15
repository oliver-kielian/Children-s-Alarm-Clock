export function utf8Encode(str) {
    return new TextEncoder().encode(str);
}