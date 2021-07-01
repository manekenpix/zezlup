/* eslint-disable @typescript-eslint/no-unused-vars */
/* eslint-disable @typescript-eslint/no-empty-interface */
import {AttributifyAttributes} from 'windicss/types/jsx';

declare module 'react' {
  interface HTMLAttributes<T> extends AttributifyAttributes {}
}
