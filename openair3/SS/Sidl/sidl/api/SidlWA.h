/*
 * Copyright 2022 Sequans Communications.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

// WA for already defined None somewhere (which conflicts with SIDL code)
#ifdef None
#undef None /* in case IDL is compiled with OAI eNB None type is also used by OAI source code */
#endif

// WA for already define C_RNTI somewhere (which conflicts with SIDL code)
#ifdef C_RNTI
#undef C_RNTI /* in case IDL is compiled with OAI eNB None type is also used by OAI source code */
#endif

